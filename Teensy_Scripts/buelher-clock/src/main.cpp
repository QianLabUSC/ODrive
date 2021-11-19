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
#include "run_stance.h"
#include "run_zero.h"

// ! Robot Configuration.
const RoboConfig Robot =
    RoboConfig(LegConfig(&Serial1, 0, 0.0f,
                         false),  // right_fore
               LegConfig(&Serial1, 1, 0.0f,
                         true),  // left_fore
               LegConfig(&Serial2, 0, 0.0f,
                         false),  // right_hind
               LegConfig(&Serial2, 1, 0.0f,
                         true),  // left_hind
               {std::make_pair(ODriveArduino(Serial1), &Serial1),
                std::make_pair(ODriveArduino(Serial2), &Serial2)});

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
        case 'c': {
            run_clock(Robot, BOUNDING);
            break;
        }
        case 'z': {
            run_zero(Robot);
            break;
        }
        case 's': {
            run_stance(Robot, STANCE);
            break;
        }
    }
}
