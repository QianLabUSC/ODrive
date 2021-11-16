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
    return;
}

void LegConfig::legSetup(int calibration_mode)
{
    for (int axis = 0; axis < 2; axis++)
    {
        /**
         *  RUNS MOTOR CALIBRATION FOR EACH MOTOR IN SEQUENCE
         *  Performs a control lockout until calibration ends.
         */
        _odrv.first.run_state(axis, calibration_mode, true);
        //*(_odrv.second) << "w axis" << axis << ".controller.input_mode " << 3 << "\n";

        // * ERROR CHECK SEQUENCE
        if (ErrorCheck(axis))
        {
            if (axis == 0) {
                Serial.println("Error in Motor Axis 0");
            } else {
                Serial.println("Error in Motor Axis 1");
            }
            return;
        }
    }

    // Puts both motors into Closed Loop Control
    for (int axis = 0; axis < 2; axis++)
    {
        _odrv.first.run_state(axis, 8, true);
    }

    Serial.println(
        "\x1b[1;37;42m"
        "**********************************\n"
        "**** CONFIGURATION SUCCESSFUL ****\n"
        "****   Motor Armed & Ready    ****\n"
        "**********************************"
        "\x1b[0m\n");

    Serial.println(
        "**********************************\n"
        "********  Command Menu:  *********\n"
        "**********************************\n"
        "	'r' -> execute radial leg movement\n"
        "   's' -> execute sinusoidal leg movement\n"
        "   'e' -> move legs to horizontal position\n"
        "   'w' -> restart ODrive Board\n"
        "	'q' -> exit");
    return;
}

void LegConfig::setup(int calibration_mode)
{
    serialSetup();
    legSetup(calibration_mode);
    return;
}

bool LegConfig::ErrorCheck(int axis)
{
    int errorNum;
    *(_odrv.second) << "r axis" << axis << "error\n";
    errorNum = _odrv.first.readInt();
    return errorNum;
}

void LegConfig::EStop() {
    *(_odrv.second) << "w axis" << 0 << ".requested_state " << 1 << '\n';
    *(_odrv.second) << "w axis" << 1 << ".requested_state " << 1 << '\n';
    return;
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

