#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <TimeLib.h>
#include <string>
#include <util.h>

// TORQUE ESTIMATION CONSTANTS
float ks = 23.8732; //proportional gain (N*m/rad)
float tau = 0.0616; //torque constant (N*m/A)
float R = 0.085;	//phase resistance (Ohms)

float torqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0);
void printTorqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate = 0);