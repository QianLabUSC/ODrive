#include "SinusoidalGait.hpp"

#define DEBUG

const float L_MAX = 0.3;
const float L_MIN = 0.03;
const float GAMMA_MIN = 0.087;
const float GAMMA_MAX = 2.61;
const float THETA_MAX = 2.47;


struct GaitParams Gaits[] = {
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
};



void SinusoidalTrajectory (float t, struct GaitParams gait, float &theta, float &gamma, 
                            float&L, float &X, float &Y) {
    // Stance Phase
    double period = 1 / gait.freq;
    float t_mod = fmod(t, period);
    if (t_mod > period * gait.stance_ratio) {
        X = gait.x_amp * cosf(2 * PI * gait.freq * gait.stance_freq * t_mod);
        Y = gait.stance_depth * sinf(2 * PI * gait.freq * gait.stance_freq * t_mod) + gait.stance_height;
    }

    // Swing Phase
    else {
        X = gait.x_amp * cosf(2 * PI * gait.freq * gait.swing_freq * t_mod);
        Y = gait.stance_height - gait.swing_retraction * sinf(2 * PI * gait.freq * gait.swing_freq * t_mod);
    }

    PhysicalToAbstract(X, Y, L, theta, gamma);
}

void GaitMovement(LegConfig Leg, float t, struct GaitParams gait, 
                    float& theta, float& gamma) {
    float x, y, L;

    SinusoidalTrajectory(t, gait, theta, gamma, L, x, y);

    if (!inBounds(gamma, theta, L))
    {
        Leg.EStop();
        #if defined(DEBUG) || defined(DEBUG_HIGH)
            Serial.println("\x1b[31mERROR---------Leg sent to idle.");
            Serial.print("\x1b[0m");
        #endif
        return;
    }
    Leg.odrv().first.SetCoupledPosition(theta, gamma);
}


bool ValidateGaitParams(struct GaitParams gait) {
    int error = 0;
    
    if (gait.stance_height == NAN || gait.freq == NAN ||
        gait.stance_depth == NAN || gait.stance_ratio == NAN ||
        gait.stride_length == NAN || gait.swing_retraction == NAN) {
            error = 1;
    }
    
    if (gait.stance_freq == NAN) {
        gait.stance_freq = 1 / (2 * gait.stance_ratio);
    }

    if (gait.swing_freq == NAN) {
        gait.swing_freq = 1 / (2 - 2 * gait.stance_ratio);
    }

    if (gait.x_amp == NAN) {
        gait.x_amp = sinf(gait.stride_length / 2);
    }
    
    if (gait.stance_height + gait.stance_depth > L_MAX) {
        error = 1;
    } else if (gait.stance_height - gait.swing_retraction < L_MIN) {
        error = 2;
    } else if (gait.stride_length/2 + PI/2 > THETA_MAX) {
        error = 3;
    } else if (gait.stance_ratio <= 0 || gait.stance_ratio >= 1) {
        error = 4;
    }
    #ifdef DEBUG
        if (!error) {
            Serial.println("Invalid Gait Parameters!");
        }
        
    #endif
    
    #ifdef DEBUG_HIGH
        switch(error) {
            case 1 :
                Serial.println("\x1b[31mInvalid Stance Height or Depth.");
                break;
            case 2 :
                Serial.println("\x1b[31mInvalid Stance Height or Swing Retraction.");
                break;
            case 3 :
                Serial.println("\x1b[31mInvalid Stride Length.");
                break;
            case 4 :
                Serial.println("\x1b[31mInvalid Stance Ratio.");
                break;
        }
    #endif
    Serial.print("\x1b[0m");
    return !error;
}

/**
 * ! Leg Workspace Must be validated empirically
 * Gamma must be within [0.087, 2.61] radians
 * Theta must be within [-2.47, +2.47] radians
 */

/**
 * @brief Checks whether a current abstract leg position is valid
 */
bool inBounds(float Gamma, float Theta, float L)
{
    int error = 0;
    if (Gamma <= GAMMA_MIN || Gamma > GAMMA_MAX) {
        error = 1;
    } else if (abs(Theta) > THETA_MAX) {
        error = 2;
    } else if (L <= L_MIN || L >= L_MAX) { // ! CALCULATE PRECISE L RANGE FROM GAMMA
        error = 3;
    }
    
    #ifdef DEBUG
        if (!error) {
            Serial.println("Leg out of bounds.");
        }
    #endif

    #ifdef DEBUG_HIGH
        if (error == 1) {
            Serial.println("\x1b[31mGamma Value is Invalid");
        } else if (error == 2) {
            Serial.println("\x1b[31mTheta Value is Invalid");
        } else if (error == 3) {
            Serial.println("\x1b[31mL Value is Invalid");
        }
    #endif
    Serial.print("\x1b[0m");
    return !error;
}

/**
 * @brief Checks whether a toe position is in bounds
 */
bool inBounds(float x, float y)
{
    float L = sqrtf(pow(x, 2) + pow(y, 2));
    
    if (L <= L_MIN || L >= L_MAX) { // ! CALCULATE PRECISE L RANGE FROM GAMMA
        return false;
    } else {
        return true;
    }
    
}

