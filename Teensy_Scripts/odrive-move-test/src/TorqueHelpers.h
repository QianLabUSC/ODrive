#ifndef TORQUEHELPERS_H
#define TORQUEHELPERS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <TimeLib.h>
#include <string>
#include "UtilityHelpers.hpp"


// TORQUE ESTIMATION CONSTANTS
const float ks = 23.8732; //proportional gain (N*m/rad)
const float tau = 0.0616; //torque constant (N*m/A)
const float R = 0.085;	//phase resistance (Ohms)

void formatTime(char *out);
float torqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate);
void printTorqueEst(ODriveArduino odrive, HardwareSerial odrive_serial, int polling_rate);

bool checkError(int axis, ODriveArduino odrive, HardwareSerial odrive_serial);

#endif