#include "TorqueHelpers.hpp"

/**
 * @param out: C-string pointer
 * 
 * @brief
 * 		fills @param out with current time in the format [minutes:seconds:milliseconds]
 */
void formatTime(char *out)
{
	int minutes = minute();
	int seconds = second();
	unsigned long time = millis();
	int ms = int(time % 1000);

	char buffer[12];

	sprintf(buffer, "%d:%d:%d", minutes, seconds, ms);

	for (int i = 0; i < 10; i++)
	{
		out[i] = buffer[i];
	}
}

/**
 * @param polling_rate: if called in loop, will determine the polling rate of the loop (through delay)
 * @param odrive: ODriveArduino object
 * @param odrive_serial: serial object connection to ODrive
 * 
 * @return float value representing torque estimate (N*m)
 * 
 * @brief
 * 		calculates the estimated external torque on the motor
 */
float torqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0)
{
	delay(polling_rate); //defines polling rate (delay_ ms)
	char time[12];

	formatTime(time); //gets the time (minutes:seconds:milliseconds)

	// //get motor setpoint
	// odrive_serial << "r axis" << 0 << ".controller.pos_setpoint\n";
	// float setpoint = odrive.readFloat();

	// //get the actual position from the encoder
	// odrive_serial << "r axis" << 0 << ".encoder.pos_circular\n";
	// float actualpos = odrive.readFloat();

	// //calculate torque estimate
	// return (float)((ks * tau * (setpoint - actualpos)) / R);
	return (float)(ks * tau * (odrive.GetPosDiff(0))) / R;
}

/**
 * @param polling_rate: if called in loop, will determine the polling rate of the loop (through delay)
 * @param odrive: ODriveArduino object
 * @param odrive_serial: serial object connection to ODrive
 * 
 * @return float value representing torque estimate (N*m)
 * 
 * @brief
 * 		calculates the estimated external torque on the motor and prints data to Serial bus
 */
float printTorqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0)
{
	delay(polling_rate); //defines polling rate (delay_ ms)
	char time[12];

	formatTime(time); //gets the time (minutes:seconds:milliseconds)

	float extTorque = (ks * tau * (odrive.GetPosDiff(0))) / R;

	float vel = odrive.GetVelocity(0);

	Serial.printf("| Time: %-8s | Torque: %-6.4f | Vel: %-6.4f |\n", time, extTorque, vel);

	return extTorque;
}

/**
 * @param axis: axis number to check
 * @param odrive: ODriveArduino object
 * @param odrive_serial: serial object connection to ODrive
 * 
 * @brief checks the ODrive Axis for errors
 * @return 	TRUE: there is an error in @param axis
 * 			FALSE: there is no error in @param axis
 */ 
bool checkError(int axis, ODriveArduino odrive, HardwareSerial odrive_serial)
{
	int errorNum;
	odrive_serial << "r axis" << axis << "error\n";
	errorNum = odrive.readInt();
	return errorNum;
}