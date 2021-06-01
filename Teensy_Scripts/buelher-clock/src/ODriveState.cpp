#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdio>

#include "UtilityHelpers.hpp"
#include "ODriveState.hpp"

void loop_control(char c, ODriveArduino odrive)
{
    int requested_state;

    requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
    Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
    if (!odrive.run_state(0, requested_state, false /*don't wait*/))
        return;
}

void idle_state(char c, ODriveArduino odrive)
{
    int requested_state;

    requested_state = ODriveArduino::AXIS_STATE_IDLE;
    Serial << "Axis" << c << ": Requesting state: IDLE (1)" << '\n';
    if (!odrive.run_state(0, requested_state, false))
        return;
}