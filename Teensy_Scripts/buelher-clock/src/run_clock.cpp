#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdio>

#include "TorqueHelpers.hpp"
#include "ODriveState.hpp"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"
#include "legs.hpp"

#include "run_clock.hpp"

// max duration in milliseconds
#define dur 50000

// !Note in progress.
float getPosition(
    Leg leg,
    RoboConfig conf, // physical robot configuration
    Gait gait,
    const BuelherClock &clock,
    long elapsed,
    int wrap = INT32_MAX)
{

    float s_elapsed = float(elapsed) / 1000.0f;

    // calculate time offset from leg phase
    

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
        angle += clock.theta_i + ((s_elapsed - clock.time_i()) * clock.omega_slow());
    else
        angle += clock.theta_f + ((s_elapsed - clock.time_f()) * clock.omega_fast());

    // wrap angle around
    angle = fmod(angle, float(360 * wrap));

    return angle;
}

void run_clock(RoboConfig conf, Gait gait, ODriveArduino odrive)
{
    Serial.println("Executing Buelher Clock. Send 'q' to stop.");

    // MUST enter closed loop mode before starting movement
    // TODO: set all 4 legs
    loop_control(0, odrive);
    loop_control(1, odrive);

    bool cont = true; // flag boolean to kill loop
    char time[12];    // time string

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
            break;

        float ref_angle = EXAMPLE.getPosition(elapsed, 1);
        float ref_rots = (1.0f / 360.0f) * ref_angle;

        odrive.SetPosition(0, ref_rots);
        odrive.SetPosition(1, ref_rots);

        formatTime(time); //gets the time (minutes:seconds:milliseconds)
        Serial << "| " << elapsed / 1000.0f << "| " << ref_angle << "| " << ref_rots << "\n";
        
        // !Note: In Progress
        float TEMP1 = getPosition(right_fore, conf, gait, EXAMPLE, elapsed);
        float TEMP2 = getPosition(right_hind, conf, gait, EXAMPLE, elapsed);
    }

    // return motor to idle state on interrupt or completion
    // TODO: set all 4 legs
    idle_state(0, odrive);
    idle_state(1, odrive);

    Serial << "DONE\n";
}
