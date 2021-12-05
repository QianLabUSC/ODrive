#include "mode_facilitator.hpp"


void RadialGait(LegConfig Leg) {
    float time;
    float ms = millis();
    float start = ms / 1000.0;

    // ? start at neutral leg position?
    float theta;
    float gamma;
    
    Serial.println("Enter Mode #: ");
    while (Serial.available() == false) {}
    int mode = Serial.parseInt();

    while (Serial.read() != 'q') {
        time = ms - start;
        RadialLegMovement(Leg, time, radial_gait_params[mode], theta, gamma);
        delay(1);
        ms = millis() / 1000.0;
    }
}

void SinusoidalGait(LegConfig Leg) {
    float time;
    float ms = millis();
    float start = ms / 1000.0;

    // ? start at neutral leg position?
    float theta;
    float gamma;
    
    Serial.println("Enter Gait #: ");
    while (Serial.available() == false) {}
    int mode = Serial.parseInt();
    
    if(ValidateGaitParams(Gaits[mode])) {
        return;
    }
    
    while (Serial.read() != 'q') {
        time = ms - start;
        GaitMovement(Leg, time, Gaits[mode], theta, gamma);
        delay(1);
        ms = millis() / 1000.0;
    }
}