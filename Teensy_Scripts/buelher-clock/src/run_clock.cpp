#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdio>

#include "TorqueHelpers.hpp"
#include "ODriveState.hpp"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"

#include "run_clock.hpp"

// max duration in milliseconds
#define dur 50000

void run_clock(Gait gait, ODriveArduino odrive)
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
    }
    
    // return motor to idle state on interrupt or completion
    // TODO: set all 4 legs
    idle_state(0, odrive);
    idle_state(1, odrive);

    Serial << "DONE\n";
}