#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <TimeLib.h>
#include <string>

// Local Imports
#include "drive_config.hpp"
#include "inline_print.hpp"

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
   */
    configure(odrive_serial, 0, &TEST_CONFIG);

    // serial monitor interface
    Serial.println("Ready!");
    Serial.println("Send the character '0' or '1' to calibrate respective motor (you must do this before you can command movement)");
    Serial.println("Send the character 'm' to set mode.");
    Serial.println("Send the character 's' to execute test move");
    Serial.println("Send the character 'r' to execute proprioceptive move test");
    Serial.println("Send the character 'b' to read bus voltage");
    Serial.println("Send the character 'k' to show position and torque information in a loop");
    Serial.println("Send the character 'p' to read motor positions in a 10s loop");
    Serial.println("Send the character 'b' to enter Buelher clock mode");
    Serial.println("Send 'q' to exit");
}

/**
 * @brief: outputs the current time in format "minutes:seconds.milliseconds"
 */
void formatTime(char *out)
{
    int minutes = minute();
    int seconds = second();
    unsigned long time = millis();
    int ms = int(time % 1000);

    char buffer[12];

    sprintf(buffer, "%d:%d.%d", minutes, seconds, ms);

    for (int i = 0; i < 10; i++)
    {
        out[i] = buffer[i];
    }
}

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
		 * @input: 'm'
		 * @brief: sets the motor state.
		 * !NOTE: This function has questionable functionality
		 */
        if (c == 'm')
        {
            Serial.println("Select desired motor state:");
            Serial.println("Idle: '1'");
            Serial.println("Closed Loop Control: '2'");
            //Serial.println("");
            int in = Serial.read();
            int requested_state;

            switch (in)
            {
            case 2: //CLOSED LOOP CONTROL
                requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
                Serial << "Axis 0: Requesting Closed Loop Control.\n";
                if (!odrive.run_state(0, requested_state, false))
                    return;
                break;
            case 1: //IDLE (default case)
                requested_state = ODriveArduino::AXIS_STATE_IDLE;
                Serial << "Axis 0: Requesting State Idle.\n";
                if (!odrive.run_state(0, requested_state, false))
                    return;
                break;
            }
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
                delay(5);
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
            char time[12];
            float ks = 23.8732; //proportional gain (N*m/rad)
            float tau = 0.0616; //torque constant (N*m/A)
            float R = 0.085;    //phase resistance (Ohms)
            while (Serial.read() != 'q')
            {
                delay(50);        //defines polling rate (50ms)
                formatTime(time); //gets the time (minutes:seconds:milliseconds)

                //get current setpoint
                odrive_serial << "r axis" << 0 << ".controller.pos_setpoint\n";
                float setpoint = odrive.readFloat();

                //get the actual position from the encoder
                odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";
                float actualpos = odrive.readFloat();

                //calculate torque estimate
                float extTorque = (ks * tau * (setpoint - actualpos)) / R;

                Serial << "| " << time << " | " << setpoint << "  | " << actualpos << "  | " << extTorque << " |\n";
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
            int dir = 1;
            bool cont = true;
            float err = 0.3;
            //if (s == 's') {
            while (cont)
            {
                Serial.println("Running...");
                if (Serial.read() == 'q')
                {
                    cont = false;
                    continue;
                }
                float vel = .25 * dir;
                odrive.SetVelocity(0, vel);
                bool contact = false;
                while (!contact)
                {
                    float ks = 23.8732; //proportional gain
                    float tau = 0.0616; //torque constant
                    float R = 85;

                    odrive_serial << "r axis" << 0 << ".controller.pos_setpoint\n";
                    float setpoint = odrive.readFloat();

                    odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";
                    float actualpos = odrive.readFloat();

                    /**
             * @brief Implementing torque estimation equation (1) from Kenneally
             * "Actuator Transparency and the Energetic Cost of Proprioception"
             */
                    float extTorque = (ks * tau * (setpoint - actualpos)) / R;
                    if (abs(extTorque) > err)
                    {
                        contact = true;
                        dir *= -1;
                    }
                }
            }
            // }
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
		 * @input: 't'
		 * @brief: runs a velocity setting test
		 */
        if (c == 't')
        {
            Serial.println("Velocity Test");
            odrive_serial << "w axis" << 0 << "controller.config.control_mode"
                          << "CONTROL_MODE_VELOCITY_CONTROL"
                          << "\n";
            odrive.SetVelocity(0, 1);
        }

        /**
		 * @input: 'b'
         * !Note: In Development
		 * @brief: runs a Buelher Clock
		 */
        if (c == 'b')
        {
            Serial.println("Buelher Clock");
        }
    }
}
