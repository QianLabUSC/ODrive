#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "ODriveState.hpp"
#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"
#include "calibrate.hpp"
#include "robo_config.hpp"
#include "run_clock.hpp"

////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

// HardwareSerial &odrive_serial_1 = Serial1;
HardwareSerial &odrive_serial_1 = Serial1;
HardwareSerial &odrive_serial_2 = Serial2;

// TODO: Remove this
// ODrive object
ODriveArduino odrive1(odrive_serial_1);
ODriveArduino odrive2(odrive_serial_2);

// !Note: Define Configurations Here:

// 21.08.12 testing config with 2 motors
const RoboConfig CONFIG = RoboConfig(
    LegConfig(std::make_pair(odrive1, Serial1), 0, 0.0f, false),  // right_fore
    LegConfig(std::make_pair(odrive1, Serial1), 1, 0.0f, true),   // left_fore
    LegConfig(std::make_pair(odrive2, Serial2), 0, 0.0f, false),  // right_hind
    LegConfig(std::make_pair(odrive2, Serial2), 1, 0.0f, true),   // left_hind
    {std::make_pair(odrive1, odrive_serial_1),
     std::make_pair(odrive2, odrive_serial_2)});

void setup() {
    // TODO: Config in function
    // ODrive uses 115200 baud
    odrive_serial_1.begin(115200);
    odrive_serial_2.begin(115200);

    // Serial to PC
    Serial.begin(115200);
    while (!Serial)
        ;  // wait for Arduino Serial Monitor to open

    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");
    Serial.println(odrive1.getBoardInfo());  // prints the firmware version of
                                             // the ODrive (confirms connection)

    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");
    Serial.println(odrive2.getBoardInfo());  // prints the firmware version of
                                             // the ODrive (confirms connection)

    // TODO: formalize setup within RoboConfig. Hard coded for now.
    calibrate(0, odrive1);  // Startup Calibration for ODrive
    calibrate(1, odrive1);  // Startup Calibration for ODrive
    odrive_serial_1 << "w axis" << 0 << ".controller.input_mode " << 3 << "\n";
    odrive_serial_1 << "w axis" << 1 << ".controller.input_mode " << 3 << "\n";
    if (checkError(0, odrive1, odrive_serial_1))
        Serial.println("Error in Motor Axis 0");
    if (checkError(1, odrive1, odrive_serial_1))
        Serial.println("Error in Motor Axis 1");

    // TODO: formalize setup within RoboConfig. Hard coded for now.
    calibrate(0, odrive2);  // Startup Calibration for ODrive
    calibrate(1, odrive2);  // Startup Calibration for ODrive
    odrive_serial_2 << "w axis" << 0 << ".controller.input_mode " << 3 << "\n";
    odrive_serial_2 << "w axis" << 1 << ".controller.input_mode " << 3 << "\n";
    if (checkError(0, odrive2, odrive_serial_2))
        Serial.println("Error in Motor Axis 0");
    if (checkError(1, odrive2, odrive_serial_2))
        Serial.println("Error in Motor Axis 1");

    // serial monitor interface
    Serial.println(
        "Motor Armed & Ready\n"
        "Command Menu:\n"
        "	'l' -> enter closed loop control.\n"
        "	'b' -> reads bus voltage\n"
        "	'q' -> Sends motors to IDLE STATE\n"
        "	'c' -> Execute Buelher Clock\n");
}

// MAIN CONTROL LOOP
void loop() {
    while (Serial.available() == false) return;

    char c = Serial.read();

    switch (c) {
        /**
         * @input: 0 or 1
         * @brief: Run calibration sequence
         */
        case '0':
        case '1':
            calibrate(c - '0', odrive1);  // convert char to int
            break;

        /**
         * @input: 'l'
         * @brief: starts closed loop control
         */
        case 'l':
            loop_control(c, odrive1);
            break;

        // Read bus voltage
        case 'b':
            odrive_serial_1 << "r vbus_voltage\n";
            Serial << "Vbus voltage: " << odrive1.readFloat() << '\n';
            break;

        /**
         * @input: 'c'
         * !Note: In Development
         * @brief: runs a Buelher Clock
         */
        case 'c':
            run_clock(CONFIG, BOUNDING, odrive1, odrive2);
            break;

        /**
         * @input: 'q'
         * @brief: sets motor state to IDLE
         */
        case 'q':
            idle_state(c, odrive1);
            break;
    }
}
