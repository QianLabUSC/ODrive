#include "run_stance.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"

void run_stance(RoboConfig conf, Gait gait) {
    Serial.println("Executing Stance. Send 'q' to stop.");

    // MUST enter closed loop mode before starting movement.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL);
    }

    /**
     * Loop Until Q is pressed.
     */
    while (true) {
        if (Serial.read() == 'q') break;

        conf[right_fore].setPosition(float(gait.right_fore) / 360.0);
        conf[right_hind].setPosition(float(gait.right_hind) / 360.0);
        conf[left_fore].setPosition(float(gait.left_fore) / 360.0);
        conf[left_hind].setPosition(float(gait.left_hind) / 360.0);
    }

    // Idle Motors.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_IDLE);
    }

    Serial << "DONE\n";
}
