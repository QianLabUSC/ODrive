#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <string>

#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"

////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

HardwareSerial &odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// *GLOBAL VARIABLES
// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;

int WORKSPACE = 0.75f;

void setup()
{
	// ODrive uses 115200 baud
	odrive_serial.begin(115200);

	// Serial to PC
	Serial.begin(115200);
	while (!Serial)
		; // wait for Arduino Serial Monitor to open

	Serial.println("ODriveArduino");
	Serial.println(odrive.getBoardInfo()); //prints the firmware version of the ODrive (confirms connection)
	Serial.println("Setting parameters...");

	odrive_serial << "config.brake_resistance " << 0.5f << '\n';
	odrive_serial << "config.dc_max_negative_current " << 3 << '\n'; 
	//odrv0.config.max_regen_current = 4
	odrive_serial << "config.max_regen_current " << 3 << '\n'; 

	odrive_serial << "w axis" << 0 << ".motor.config.pre_calibrated " << true << '\n';

	//odrive.motor_calibrated(0);
	/**
   * *Motor configuration setup. 
   * Configured for the T-Motor U8II KV150 w/ AS5048A rotary encoder.
   */
	for (int axis = 0; axis < NUM_MOTORS; ++axis)
	{
		if (odrive.motor_calibrated(axis)) {
			Serial.println("Motor pre-calibrated!");
			odrive_serial << "sr\n";
		}
		else {
			Serial.println("Motor calibration underway...");
			/**
			 * U8II Motor and Encoder Configuration Setup
			 * The config commands end up writing something like "w axis0.motor.config.current_lim 10.0\n"
			 **/

			odrive_serial << "w axis" << axis << ".controller.config.vel_limit " << 50.0f << '\n';
			odrive_serial << "w axis" << axis << ".motor.config.current_lim " << 4.20f << '\n';
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

			odrive_serial << ".ss" << '\n';

			/**
			 * Initial Calibration for ODrive
			 */
			int requested_state;
			requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
			Serial << "Axis" << axis << ": Requesting state " << requested_state << '\n';
			odrive.run_state(axis, requested_state, true);

			/**
			 * Sets up ODrive Axis to be configured for next run.
			 */
			odrive_serial << "w axis" << axis << ".motor.config.pre_calibrated " << true << '\n';
			odrive_serial << "w axis" << axis << ".config.startup_encoder_offset_calibration " << 1 << '\n';
			odrive_serial << "w axis" << axis << ".config.startup_closed_loop_control " << 1 << '\n';
			
			//Changes motor controller input mode to input PASSTHROUGH mode
			odrive_serial << "w axis" << axis << ".controller.input_mode " << 1 << '\n';
			
			// Change motor to Circular Position Setpoints
			odrive_serial << "w axis " << axis << ".controller.config.circular_setpoints " << true << '\n';
			odrive_serial << "w axis " << axis << ".controller.config.circular_setpoints_range " << 0.75f << '\n';

			odrive_serial << "ss\n";
			odrive_serial << "sr\n";
		}
		// Resets the motor position to Zero
		odrive.SetPosition(axis, 0.1);
	}

	// Calibration Error Checks
	if (checkError(0, odrive, odrive_serial))
	{
		Serial.println("Error in Motor Axis 0");
	}
	else if (checkError(1, odrive, odrive_serial))
	{
		Serial.println("Error in Motor Axis 1");
	} 
	else 
	{
		Serial.println("**********************************");
		Serial.println("*****CONFIGURATION SUCCESSFUL*****");
		Serial.println("**********************************");
	}

	// serial monitor interface
	Serial.println("\nMotor Armed & Ready");
	Serial.println("Command Menu:");
	Serial.println("	'0' or '1' -> calibrate respective motor");
	Serial.println("	'l' -> enter closed loop control.");
	Serial.println("	's' -> execute test movement");
	Serial.println("	'r' -> execute proprioceptive move test");
	Serial.println("	'b' -> reads bus voltage");
	Serial.println("	'k' -> shows position and torque information in a loop");
	Serial.println("	'p' -> reads motor positions in a 10s loop");
	Serial.println("	'q' -> Sends motors to IDLE STATE");

	for (int i = 0; i < 5; i++) 
	{
		float dump = odrive.readFloat();
		Serial.println("Buffer Clear: ");
		Serial.printf("%.6f \n", dump);
	}
	


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
		 * @brief: Runs a test movement on motor 0, stopping when contact is detected
		 */
		if (c == 's')
		{
			for (int i = 0; i < 3; i++) 
			{
				float dump = odrive.readFloat();
				//Serial.println("Buffer Clear: ");
				Serial.printf("%.6f \n", dump);
			}
			Serial.println("Executing test move");
			for (float ph = 0.10f; ph < 1.75f; ph += 0.015f)
			{
				float pos_m0 = ph;
				//float pos_m1 = fmod(2.0f * sin(ph), WORKSPACE);
				//Serial.printf("Position: %.3f", pos_m0);
				odrive.SetPosition(0, pos_m0);
				//odrive.SetPosition(1, pos_m1);

				delay(1);

				float torque = printTorqueEst(odrive, odrive_serial, 0);
				float angleDeviance = odrive.GetPosDiff(0);
				if (torque > 0.3f || angleDeviance > 0.3f)
				{
					Serial.printf("Exit Torque: %f\n", torque);
					//Serial.println("\n");
					//printTorqueEst(odrive, odrive_serial, 0);
					odrive.SetPosition(0, pos_m0 - 0.02f);
					ph = 2;
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
		* @param 'k'
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
			odrive.SetPosition(0, 0);
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
			//odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";

			int i = 0;
			Serial << "|   TIME   |  SET POS  | ACTUAL POS | EST TORQUE |  VELOCITY  |\n";
			for (float ph = 0.0f; ph < 1.5f; ph += 0.01f)
			{
				if (checkError(0, odrive, odrive_serial))
				{
					Serial.println("Error in Motor Axis 0");
					break;
				}
				float pos_m0 = fmod(ph, WORKSPACE);
				//float pos_m0 = 2.0f * sin(ph);
				//float pos_m1 = 2.0f * sin(ph);

				// if (i % 50 == 0) {
				//  	printTorqueEst(0);
				// }
				i++;
				printTorqueEst(odrive, odrive_serial, 5);
				odrive.SetPosition(0, pos_m0);
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
		 * @input: 'z'
		 * @brief: sets motor position to zero
		 */
		if (c == 'z')
		{
			odrive.SetPosition(0, 0.10);
		}

		/**
		 * @input: '.'
		 * @brief: clears board errors
		 */
		if (c == '.')
		{
			odrive_serial << "sc\n";
		}

		// Read bus voltage
		if (c == 'v')
		{
			float vel = odrive.GetVelocity(0);
			Serial << "Motor 0 Velocity: " << vel << '\n';
		}

	}
}
