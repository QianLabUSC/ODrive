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
const RoboConfig CONFIG =
    RoboConfig(LegConfig(std::make_pair(odrive1, odrive_serial_1), 0, 0.0f,
                         false),  // right_fore
               LegConfig(std::make_pair(odrive1, odrive_serial_1), 1, 0.0f,
                         true),  // left_fore
               LegConfig(std::make_pair(odrive2, odrive_serial_2), 0, 0.0f,
                         false),  // right_hind
               LegConfig(std::make_pair(odrive2, odrive_serial_2), 1, 0.0f,
                         true),  // left_hind
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

    CONFIG.run_config();
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
