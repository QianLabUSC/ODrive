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
const RoboConfig Robot =
    RoboConfig(LegConfig(std::make_pair(odrive1, odrive_serial_1), 0, 0.0f,
                         false),  // right_fore
               LegConfig(std::make_pair(odrive1, odrive_serial_1), 1, 0.0f,
                         true),  // left_fore
               LegConfig(std::make_pair(odrive2, odrive_serial_2), 0, 0.0f,
                         false),  // right_hind
               LegConfig(std::make_pair(odrive2, odrive_serial_2), 1, 0.0f,
                         true),  // left_hind
               {std::make_pair(odrive1, &odrive_serial_1),
                std::make_pair(odrive2, &odrive_serial_2)});

void setup() { Robot.setup(); }

// MAIN CONTROL LOOP
void loop() {
    while (Serial.available() == false) return;

    char c = Serial.read();

    // Currently, no other modes are needed.
    switch (c) {
        /**
         * @input: 'c'
         * !Note: In Development
         * @brief: runs a Buelher Clock
         */
        case 'c':
            run_clock(Robot, BOUNDING, odrive1, odrive2);
            break;
    }
}
