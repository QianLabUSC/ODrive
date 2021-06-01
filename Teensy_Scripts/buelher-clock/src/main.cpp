#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <string>

#include "TorqueHelpers.h"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"
#include "ODriveState.hpp"

////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

HardwareSerial &odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;

void setup()
{
    // ODrive uses 115200 baud
    odrive_serial.begin(115200);

    // Serial to PC
    Serial.begin(115200);
    while (!Serial)
        ; // wait for Arduino Serial Monitor to open

    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");
    Serial.println(odrive.getBoardInfo()); //prints the firmware version of the ODrive (confirms connection)

    /**
   * *Motor configuration setup. 
   * Configured for the T-Motor U8II KV150 w/ AS5048A rotary encoder.
   */
    for (int axis = 0; axis < NUM_MOTORS; ++axis)
    {
        /**
		 * U8II Motor and Encoder Configuration Setup
		 * The config commands end up writing something like "w axis0.motor.config.current_lim 10.0\n"
		 **/
        odrive_serial << "w axis" << axis << ".controller.config.vel_limit " << 20.0f << '\n';
        
        /**
         * Change to circular (rather than absolute) setpoints.
         * Docs: https://docs.odriverobotics.com/#circular-position-control
         */
        odrive_serial << "w axis" << axis << ".controller.config.circular_setpoints " << true << '\n';
        
        odrive_serial << "w axis" << axis << ".motor.config.current_lim " << 4.0f << '\n';
        odrive_serial << "w axis" << axis << ".motor.config.pole_pairs" << 21 << '\n';
        odrive_serial << "w axis" << axis << ".motor.config.torque_constant" << 0.061f << '\n';
        odrive_serial << "w axis" << axis << ".motor.config.motor_type"
                      << "MOTOR_TYPE_HIGH_CURRENT" << '\n';
        odrive_serial << "w axis" << axis << ".encoder.config.abs_spi_cs_gpio_pin" << 7 + axis << '\n';
        odrive_serial << "w axis" << axis << ".encoder.config.mode"
                      << "ENCODER_MODE_SPI_ABS_AMS" << '\n';
        odrive_serial << "w axis" << axis << ".encoder.config.cpr" << 16384 << '\n';

        /**
		 * OLD TUNING VALUES
		 * 		Pos_Gain = 186.7
		 * 		Vel_Gain = 0.304
		 * 		Vel_Int_Gain = 0.893
		 */
        odrive_serial << "w axis" << axis << ".controller.config.pos_gain" << 150 << '\n';
        odrive_serial << "w axis" << axis << ".controller.config.vel_gain" << 0.304f << '\n';
        odrive_serial << "w axis" << axis << ".controller.config.vel_integrator_gain" << 1.5f << '\n';
        odrive_serial << ".save_configuration()" << '\n';
        odrive_serial << ".reboot()" << '\n';

        /**
		 * Startup Calibration for ODrive
		 */
        int requested_state;
        requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
        Serial << "Axis" << axis << ": Requesting state " << requested_state << '\n';
        odrive.run_state(axis, requested_state, true);

        // Changes motor controller input mode to input PASSTHROUGH mode
        odrive_serial << "w axis" << axis << ".controller.input_mode " << 3 << "\n";

        if (checkError(0, odrive, odrive_serial))
        {
            Serial.println("Error in Motor Axis 0");
        }
    }

    // serial monitor interface
    Serial.println("Motor Armed & Ready");
    Serial.println("Command Menu:");
    Serial.println("	'0' or '1' -> calibrate respective motor (you must do this before you can command movement)");
    Serial.println("	'l' -> enter closed loop control.");
    Serial.println("	's' -> execute test movement");
    Serial.println("	'b' -> reads bus voltage");
    Serial.println("	'q' -> Sends motors to IDLE STATE");
    Serial.println("	'c' -> Execute Buelher Clock");
}

// MAIN CONTROL LOOP
void loop()
{
    if (Serial.available())
    {
        char c = Serial.read();

        /**
		 * @input: 0 or 1
		 * @brief: Run calibration sequence
		 */
        if (c == '0' || c == '1')
        {
            int motornum = c - '0';
            int requested_state;

            requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            if (!odrive.run_state(motornum, requested_state, true))
                return;
        }

        /**
		 * @input: 'l'
		 * @brief: starts closed loop control
		 */
        if (c == 'l')
            loop_control(c,odrive);

        /**
		 * @input: 's'
		 * @brief: Runs a test movement on motor 0
		 */
        if (c == 's')
        {
            Serial.println("Executing test move");
            for (float ph = 0.0f; ph < 6.28318530718f; ph += 0.01f)
            {
                float pos_m0 = 2.0f * cos(ph);
                //float pos_m1 = 2.0f * sin(ph);
                odrive.SetPosition(0, pos_m0);

                //odrive.SetPosition(1, pos_m1);
                delay(50);
                if (torqueEst(odrive, odrive_serial, 0) > 0.1f)
                {
                    printTorqueEst(odrive, odrive_serial, 0);
                    break;
                }
            }
        }

        // Read bus voltage
        if (c == 'b')
        {
            odrive_serial << "r vbus_voltage\n";
            Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
        }

        /**
		 * @input: 'q'
		 * @brief: sets motor state to IDLE
		 */
        if (c == 'q')
            idle_state(c, odrive);

        /**
		 * @input: 'b'
         * !Note: In Development
		 * @brief: runs a Buelher Clock
		 */
        if (c == 'c')
        {
            Serial.println("Executing Buelher Clock. Send 'q' to stop.");

            // MUST enter closed loop mode before starting movement
            loop_control(c, odrive);

            // max duration in milliseconds
            const long dur = 50000;

            // flag boolean to kill loop
            bool cont = true;

            // time string
            char time[12];

            // track time since loop start
            long start = millis();
            long elapsed = millis() - start;

            /**
             * Loop Until Time Elapses or Q is pressed.
             */
            while (cont && elapsed < dur)
            {
                elapsed = millis() - start;
                if (Serial.read() == 'q')
                {
                    cont = false;
                    continue;
                }

                float ref_angle = getPosition(elapsed, EXAMPLE, 1);
                float ref_rots = (1.0f / 360.0f) * ref_angle;

                odrive.SetPosition(0, ref_rots);

                formatTime(time); //gets the time (minutes:seconds:milliseconds)
                Serial << "| " << elapsed / 1000.0f << "| " << ref_angle << "| " << ref_rots << "\n";
            }
            // return motor to idle state on interrupt or completion
            idle_state(c, odrive);

            Serial << "DONE\n";
        }
    }
}
