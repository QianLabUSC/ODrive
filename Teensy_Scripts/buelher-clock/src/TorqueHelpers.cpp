#include "TorqueHelpers.h"


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


float torqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0)
{
	delay(polling_rate); //defines polling rate (delay_ ms)
	char time[12];

	formatTime(time); //gets the time (minutes:seconds:milliseconds)

	//get motor setpoint
	odrive_serial << "r axis" << 0 << ".controller.pos_setpoint\n";
	float setpoint = odrive.readFloat();

	//get the actual position from the encoder
	odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";
	float actualpos = odrive.readFloat();

	//calculate torque estimate
	return (float)((ks * tau * (setpoint - actualpos)) / R);
}

void printTorqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0)
{
	delay(polling_rate); //defines polling rate (delay_ ms)
	char time[12];

	formatTime(time); //gets the time (minutes:seconds:milliseconds)

	//get motor setpoint
	odrive_serial << "r axis" << 0 << ".controller.pos_setpoint\n";
	float setpoint = odrive.readFloat();

	//get the actual position from the encoder
	odrive_serial << "r axis" << 0 << ".encoder.pos_estimate\n";
	float actualpos = odrive.readFloat();

	//calculate torque estimate
	float extTorque = (ks * tau * (setpoint - actualpos)) / R;

	odrive_serial << "r axis" << 0 << ".encoder.vel_estimate\n";
	float vel = odrive.readFloat();

	Serial << "| " << time << " | " << setpoint << "  | " << actualpos << "  | " << extTorque << " | " << vel << " |\n";
}