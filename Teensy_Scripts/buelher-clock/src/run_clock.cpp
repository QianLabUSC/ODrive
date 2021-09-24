#include "run_clock.hpp"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include <cstdio>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "ODriveState.hpp"
#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"
#include "legs.hpp"

// max duration in milliseconds
#define dur 50000

float getRotations(Leg leg,                    // Robot's leg enum
                   RoboConfig conf,            // physical robot configuration
                   Gait gait,                  // robot gait
                   const BuelherClock &clock,  // Buelher clock parameters
                   long elapsed,               // time elapsed since start
                   int wrap = INT32_MAX) {
    float s_elapsed = float(elapsed) / 1000.0f;

    // offset time based on leg phase
    float time_offset = (gait[leg] / 360.0f) * clock.period();
    s_elapsed += time_offset;

    // whole number period
    int rotations = int(s_elapsed / clock.period());

    // fractional period
    s_elapsed = fmod(s_elapsed, clock.period());

    // whole rotations
    float angle = rotations * 360.0f;

    // add fractional rotation
    if (s_elapsed <= clock.time_i())
        angle += clock.omega_fast() * s_elapsed;
    else if (s_elapsed <= clock.time_f())
        angle +=
            clock.theta_i + ((s_elapsed - clock.time_i()) * clock.omega_slow());
    else
        angle +=
            clock.theta_f + ((s_elapsed - clock.time_f()) * clock.omega_fast());

    // add zeroing offset
    angle += conf[leg].init_offset() * 360.0f;

    // wrap angle around
    angle = fmod(angle, float(360 * wrap));

    return angle *
           (1.0f /
            360.0f);  // divide by 360 to convert from degrees to rotations
}

void run_clock(RoboConfig conf, Gait gait, ODriveArduino odrive1,
               ODriveArduino odrive2) {
    Serial.println("Executing Buelher Clock. Send 'q' to stop.");

    // MUST enter closed loop mode before starting movement.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL);
    }

    bool cont = true;  // flag boolean to kill loop
    char time[12];     // time string

    // track time since loop start
    long start = millis();
    long elapsed = millis() - start;

    /**
     * Loop Until Time Elapses or Q is pressed.
     */
    while (cont && elapsed < dur) {
        elapsed = millis() - start;
        if (Serial.read() == 'q') break;

        float ref_angle = EXAMPLE.getPosition(elapsed, 1);
        float ref_rots = (1.0f / 360.0f) * ref_angle;

        conf[right_fore].setPosition(
            getRotations(right_fore, conf, gait, EXAMPLE, elapsed));
        conf[right_hind].setPosition(
            getRotations(right_hind, conf, gait, EXAMPLE, elapsed));
        conf[left_fore].setPosition(
            getRotations(left_fore, conf, gait, EXAMPLE, elapsed));
        conf[left_hind].setPosition(
            getRotations(left_hind, conf, gait, EXAMPLE, elapsed));

        formatTime(time);  // gets the time (minutes:seconds:milliseconds)
        Serial << "| " << elapsed / 1000.0f << "| " << ref_angle << "| "
               << ref_rots << "\n";
    }

    // Idle Motors.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_IDLE);
    }

    Serial << "DONE\n";
}
