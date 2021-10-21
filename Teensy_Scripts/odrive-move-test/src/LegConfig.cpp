#include "LegConfig.hpp"

#include <Arduino.h>
#include <SoftwareSerial.h>

#include <cstdlib>
#include <cstdio>
#include <string>

LegConfig::LegConfig(HardwareSerial *serial_ptr, float axis0_init_offset,
                     float axis1_init_offset)
    : _odrv(std::make_pair(ODriveArduino(*serial_ptr), serial_ptr)),
      _axis0_init_offset(axis0_init_offset),
      _axis1_init_offset(axis1_init_offset) {}

// ODrive Serial Setup
void LegConfig::serialSetup()
{
    // *Serial Start
    _odrv.second->begin(BAUD);

    Serial.begin(BAUD);
    while (!Serial)
        ; // wait for Arduino Serial Monitor to Open

    /* Confirm ODrive Connection */
    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");

    Serial.println(_odrv.first.getBoardInfo());
}

void LegConfig::legSetup(int calibration_mode)
{
    for (int axis = 0; axis < 2; axis++)
    {
        /**
         *  RUNS MOTOR CALIBRATION FOR EACH MOTOR IN SEQUENCE
         *  Performs a control lockout until calibration ends.
         */
        _odrv.first.run_state(axis, 3, true);
        *(_odrv.second) << "w axis" << axis << ".controller.input_mode " << 3 << "\n";

        // * ERROR CHECK SEQUENCE
        if (ErrorCheck(axis))
        {
            Serial.println("Error in Motor Axis");
            return;
        }
    }
    Serial.println(
        "**********************************\n"
        "*****CONFIGURATION SUCCESSFUL*****\n"
        "**********************************");

    Serial.println(
        "Motor Armed & Ready\n"
        "Command Menu:\n"
        "	'l' -> enter closed loop control.\n"
        "	'b' -> reads bus voltage\n"
        "	'q' -> Sends motors to IDLE STATE\n"
        "	'c' -> Execute Buelher Clock\n");
}

void LegConfig::setup()
{
    serialSetup();
    legSetup();
}

bool LegConfig::ErrorCheck(int axis)
{
    int errorNum;
    *(_odrv.second) << "r axis" << axis << "error\n";
    errorNum = _odrv.first.readInt();
    return errorNum;
}

const float LegConfig::init_offset(int axis) const
{
    switch (axis)
    {
    case 0:
        return _axis0_init_offset;
        break;
    case 1:
        return _axis1_init_offset;
        break;
    }
    return 0.0f;
}