#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <math.h> /* fmod */

#include "TorqueHelpers.h"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"

////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

HardwareSerial &odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;

// get clockwork leg position at given time
// returns position (deg, 0 ≤ x ≤ 360)
float getPosition(long elapsed, BuelherClock clock)
{
    float s_elapsed = float(elapsed) / 1000.0f;

    // whole number period
    int rotations = int(s_elapsed / clock.period());

    // fractional period
    s_elapsed = fmod(s_elapsed, clock.period());

    // whole rotations
    float angle = rotations * 360.0f;

    // add fractional rotation
    if (s_elapsed <= clock.time_i())
        angle += clock.omega_fast() * s_elapsed;
    else if (s_elapsed <= clock.time_f())
        angle += clock.theta_i + ((s_elapsed - clock.time_i()) * clock.omega_slow());
    else
        angle += clock.theta_f + ((s_elapsed - clock.time_f()) * clock.omega_fast());

    return angle;
}

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
        odrive_serial << "w axis" << axis << ".motor.config.current_lim " << 15.0f << '\n';
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

        /**
		 * Changing to Closed Loop Control
		 */
        requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
        Serial << "Axis" << axis << ": Requesting state " << requested_state << '\n';
        odrive.run_state(axis, requested_state, false /*don't wait*/);

        // Changes motor controller input mode to input PASSTHROUGH mode
        odrive_serial << "w axis" << axis << ".controller.input_mode " << 1 << "\n";

        if (checkError(0, odrive, odrive_serial)) {
			Serial.println("Error in Motor Axis 0");
		}
    }

    // serial monitor interface
    Serial.println("Motor Armed & Ready");
    Serial.println("Command Menu:");
    Serial.println("	'0' or '1' -> calibrate respective motor (you must do this before you can command movement)");
    Serial.println("	'l' -> enter closed loop control.");
    Serial.println("	's' -> execute test movement");
    Serial.println("	'r' -> execute proprioceptive move test");
    Serial.println("	'b' -> reads bus voltage");
    Serial.println("	'k' -> shows position and torque information in a loop");
    Serial.println("	'p' -> reads motor positions in a 10s loop");
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
        {
            //int motornum = c-'0';
            int requested_state;

            requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            if (!odrive.run_state(0, requested_state, false /*don't wait*/))
                return;
        }

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
		 * @input: 'k'
		* @brief: Reads the encoder input and calculates a torque vector
		* 		with timestamp.
		* 
		* @output: prints in format:
		* 
		* 		[time] | [Set Pos] | [Actual Pos] | [Torque Est.]
		*/
        if (c == 'k')
        {
            Serial << "|   TIME   |  SET POS  | ACTUAL POS | EST TORQUE |\n";
            odrive_serial << "w axis" << 0 << ".controller.input_mode " << 3 << "\n";
            odrive.SetPosition(0, 0);
            odrive_serial << "w axis" << 0 << ".controller.input_mode " << 1 << "\n";
            while (Serial.read() != 'q')
            {
                printTorqueEst(odrive, odrive_serial, 0);
            }
        }

        /**
		 * @input: 'r'
		 * @brief: runs a proprioceptive test movement on motor 0.
		 * 
		 * !NOTE: this function does not work as intended
		 * @full: should rotate the motor at .25 rot/s until contact is detected, and then switch direction
		 */
        if (c == 'r')
        {
            Serial.println("Executing proprioceptive test. Send 'q' to stop.");
            //char s = Serial.read();
            odrive.SetPosition(0, 0);
            delay(4000);
            Serial.println("Starting...");
            odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";

            int i = 0;
            Serial << "|   TIME   |  SET POS  | ACTUAL POS | EST TORQUE |  VELOCITY  |\n";
            for (float ph = 0.0f; ph < 1.5f; ph += 0.01f)
            {
                //float pos_m0 = 2.0f * sin(ph);
                //float pos_m1 = 2.0f * sin(ph);

                // if (i % 50 == 0) {
                //  	printTorqueEst(0);
                // }
                delay(1);
                printTorqueEst(odrive, odrive_serial, 0);
                delay(1);
                i++;
                odrive.SetPosition(0, ph);
                //odrive.SetPosition(1, pos_m1);
            }
            Serial.println("************************");
            Serial.println("*****TEST COMPLETED*****");
            Serial.println("************************");
        }

        /**
		 * @input: 'q'
		 * @brief: sets motor state to IDLE
		 */
        if (c == 'q')
        {
            int requested_state;

            requested_state = ODriveArduino::AXIS_STATE_IDLE;
            Serial << "Axis" << c << ": Requesting state: IDLE (1)" << '\n';
            if (!odrive.run_state(0, requested_state, false))
                return;
        }

        /**
		 * @input: 'v'
		 * @brief: runs a velocity setting test
		 * ! FUNCTION DOES NOT WORK
		 */
        if (c == 'v')
        {
            // odrive_serial << "w axis" << 0 << "controller.config.control_mode " << "2" << "\n";
            // odrive_serial << "w axis" << 0 << "controller.config.input_mode " << "INPUT_MODE_PASSTHROUGH" << "\n";
            // odrive_serial << "w axis" << 0 << "controller.input_vel " << "0" << "\n";

            int requested_state;
            requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
            if (!odrive.run_state(0, requested_state, false))
                return;
            Serial.println("Velocity Test");

            //odrive_serial << "ss" << '\n';
            //odrive_serial << "sr" << '\n';
            odrive.SetVelocity(0, 3, .12);

            //odrive_serial << "odrv0.axis0.controller.input_vel " << "2" << '\n';
            //odrive_serial << "v 0 2 1 \n";
            //delay(10000);
            //odrive_serial << "w axis" << 0 << "controller.input_vel " << "0" << "\n";
        }

        /**
		 * @input: 'b'
         * !Note: In Development
		 * @brief: runs a Buelher Clock
		 */
        if (c == 'c')
        {
            Serial.println("Executing Buelher Clock. Send 'q' to stop.");

            // max duration in milliseconds
            const long dur = 5000;

            // flag boolean to kill loop
            bool cont = true;

            // time string
            char time[12];

            // track time since loop start
            long start = millis();
            long elapsed = millis() - start;

            printTorqueEst(odrive, odrive_serial, 0);
            odrive_serial << "r axis" << 0 << ".encoder.set_linear_count(0)\n";
            printTorqueEst(odrive, odrive_serial, 0);

            /**
             * Loop Until Time Elapses or Q is pressed.
             */
            while (cont && elapsed < dur)
            {
                if (checkError(0, odrive, odrive_serial)) {
					Serial.println("Error in Motor Axis 0");
					break;
				}

                elapsed = millis() - start;
                if (Serial.read() == 'q')
                {
                    cont = false;
                    continue;
                }

                // TODO: Fix Angle to not Wrap to 0
                float ref_angle = getPosition(elapsed, EXAMPLE);
                float ref_rots = (1.0f / 360.0f) * ref_angle;
                float pos_m0 = 2.0f * cos(ref_rots);

                // TEMP DISABLED
                odrive.SetPosition(0, ref_rots);

                // wait a bit
                long temp = millis();

                formatTime(time); //gets the time (minutes:seconds:milliseconds)
                Serial << "| " << elapsed / 1000.0f << "| " << ref_angle << "| " << ref_rots << "\n";
            }
            Serial << "DONE\n";
        }
    }
}
