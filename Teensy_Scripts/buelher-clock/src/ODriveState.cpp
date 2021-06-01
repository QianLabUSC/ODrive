#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdio>

#include "UtilityHelpers.hpp"
#include "ODriveState.hpp"
#include "TorqueHelpers.h" /* formatTime */
#include "buelher.hpp"

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

void buelher_clock(char c, ODriveArduino odrive, BuelherClock config)
{

    // MUST enter closed loop mode before starting movement
    loop_control(c, odrive);

    // max duration in milliseconds
    const long dur = 50000;

    // flag boolean to kill loop
    bool cont = true;

    // time string
    char time[12];

    // track time since loop start
    long start = millis();
    long elapsed = millis() - start;

    /**
             * Loop Until Time Elapses or Q is pressed.
             */
    while (cont && elapsed < dur)
    {
        elapsed = millis() - start;
        if (Serial.read() == 'q')
        {
            cont = false;
            continue;
        }

        float ref_angle = config.getPosition(elapsed, 1);
        float ref_rots = (1.0f / 360.0f) * ref_angle;

        odrive.SetPosition(0, ref_rots);

        formatTime(time); //gets the time (minutes:seconds:milliseconds)
        Serial << "| " << elapsed / 1000.0f << "| " << ref_angle << "| " << ref_rots << "\n";
    }
    // return motor to idle state on interrupt or completion
    idle_state(c, odrive);
}
