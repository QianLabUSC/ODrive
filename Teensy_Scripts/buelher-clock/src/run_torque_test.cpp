#include "run_torque_test.h"

// Printing with stream operator helper functions

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
#define dur 5000

void hard_calibration(RoboConfig conf){
*(conf[right_hind].odrv().second) << "r axis" << 0 << ".encoder.set_linear_count(0)\n";
*(conf[right_hind].odrv().second) << "r axis" << 0 << ".controller.input_pos=0\n";

*(conf[right_fore].odrv().second) << "r axis" << 1 << ".encoder.set_linear_count(0)\n";    
*(conf[right_fore].odrv().second) << "r axis" << 1 << ".controller.input_pos=0\n";

*(conf[left_hind].odrv().second) << "r axis" << 1 << ".encoder.set_linear_count(0)\n";
*(conf[left_hind].odrv().second) << "r axis" << 1 << ".controller.input_pos=0\n";

return;

}
float getRotations2(Leg leg,                    // Robot's leg enum
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

void run_torque_test(RoboConfig conf, Gait gait) {
    Serial.println("Executing Torque Test. Send 'q' to stop.");

    // MUST enter closed loop mode before starting movement.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL);
    }
    hard_calibration(conf);
    
    char time[12];  // time string

    // track time since loop start
    long start = millis();
    long elapsed = millis() - start;

    /**
     * Loop Until Time Elapses or Q is pressed.
     */
    while (elapsed < dur) {
        elapsed = millis() - start;
        if (Serial.read() == 'q') break;

        float ref_angle = EXAMPLE.getPosition(elapsed, 1);
        float ref_rots = (1.0f / 360.0f) * ref_angle;
        float control_pos= getRotations2(left_fore, conf, gait, EXAMPLE, elapsed);
        conf[right_fore].setPosition(0);
        conf[right_hind].setPosition(0);
        conf[left_fore].setPosition(
           control_pos);
        conf[left_hind].setPosition(0);

        formatTime(time);  // gets the time (minutes:seconds:milliseconds)
        //Torque [N.cm] = (8.27 * Current [A] / KV) * 100 ])
       *(conf[left_fore].odrv().second) << "r axis" << 0 << ".motor.current_control.Iq_measured\n";
	//    *(conf[left_fore].odrv().second) << "r axis" << 0 << ".encoder.pos_estimate\n";
	//    float actualpos = conf[left_fore].readFloat();
        
    //    float temp=control_pos-actualpos;
    //    while (temp> 3.1415) {
    //        temp=temp - 3.1415;
     //   }
	    float current =  conf[left_fore].readFloat();
        Serial.println(current);
    }


    // Idle Motors.
    for (auto leg : conf.legs) {
        leg.setState(ODriveArduino::AXIS_STATE_IDLE);
    }

    Serial << "DONE\n";
}