#include "run_zero.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"

void run_zero(RoboConfig conf, Gait gait = ZERO) {
    Serial.println("Executing Zero. Send 'q' to stop.");

    // MUST enter closed loop mode before starting movement.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL);
    }

    /**
     * Loop Until Q is pressed.
     */
    while (true) {
        if (Serial.read() == 'q') break;

        conf[right_fore].setPosition(0);
        conf[right_hind].setPosition(0);
        conf[left_fore].setPosition(0);
        conf[left_hind].setPosition(0);
    }

    // Idle Motors.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_IDLE);
    }

    Serial << "DONE\n";
}
