#include "PositionControl.hpp"

#include <cmath>

#define DEBUG

const float L_MAX = 0.3;
const float L_MIN = 0.03;
const float GAMMA_MIN = 0.087;
const float GAMMA_MAX = 2.61;
const float THETA_MAX = 2.47;
/**
 * ! Leg Workspace:
 * Gamma must be within [0.087, 2.61] radians
 * Theta must be within [-2.47, +2.47] radians
 */


/**
 * {L_i, L_f, theta, freq}
 */
struct RadialGaitParams radial_gait_params[] = {
    {NAN, NAN, NAN, NAN},
    {0.11f, 0.27f, PI/2, 0.5f},
    {0.13f, 0.25f, (3*PI/4), 1.0f},
    {0.13f, 0.25f, PI, 1.0f},
    {0.13f, 0.25f, PI, 0.25f},
};

// !Works off of the assumption that axis 1's angle reference is a mirror of axis 0
// !must change this in case we use an ODrive command to shift axis1's encoder reference
/**
 * @brief Returns a pair of motor angles converted to radians
 */
void GetAngles(LegConfig leg, float &angle_0, float &angle_1)
{
    angle_0 = leg.odrv().first.GetPosition(0) * 2 * PI;
    angle_1 = leg.odrv().first.GetPosition(1) * 2 * PI;

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Axis 0: %f, Axis 1: %f\n", angle_0, angle_1);
    Serial.println("\nDEBUG:: Axis Angle Outputs:");
    Serial.println(buffer);
#endif
}

/**
 * @brief Converts Abstract leg position into Theta, Gamma values
 *
 * @param L : Leg length
 * @param Theta : Abstract leg angle
 * @return : Pair (Theta, Gamma) representing the motor separation
 *             for the given input parameters
 */
void GetGamma(float L, float theta, float &gamma)
{
    float cos_param = (pow(L1, 2.0) + pow(L, 2.0) - pow(L2, 2.0)) / (2 * L1 * L);
    
    if (cos_param < -1.0) {
        gamma = PI;
        #if defined(DEBUG) || defined(DEBUG_HIGH)
        Serial.println("ERROR: L is too small to find valid alpha and beta!");
        #endif
    } else if (cos_param > 1.0) {
        gamma = 0;
        #if defined(DEBUG) || defined(DEBUG_HIGH)
        Serial.println("ERROR: L is too large to find valid alpha and beta!");
        #endif
    } else {
        gamma = acos(cos_param);
    }

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Theta: %f, Gamma: %f, Cos_Param: %f", theta, gamma, cos_param);
    Serial.println("DEBUG:: Theta Gamma Calculation:");
    Serial.println(buffer);
#endif
}

/**
 * @brief Finds the Leg length, angle, and motor separation for a given X, Y toe point
 *
 * @param X : Toe X position
 * @param Y : Toe Y position
 *
 * @return : Returns <Length, Angle, Gamma>
 */
void PhysicalToAbstract(float X, float Y, float &L, float &theta, float &gamma)
{
    L = sqrt(pow(X, 2) + pow(X, 2));
    theta = atan2(Y, X);
    gamma = (float)acos((pow(L1, 2) + pow(L, 2) + pow(L2, 2)) / (2 * L2 * L));

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.println("\nDEBUG:: Physical to Abstract:");
    Serial.println(buffer);
#endif
}

/**
 * @brief Finds the Leg angle and motor separation for a given X, Y toe point
 *
 * @param X : Toe X position
 * @param Y : Toe Y position
 *
 * @return : Returns <Theta, Gamma>
 */
void PhysicalToAbstract(float X, float Y, float &theta, float &gamma)
{
    float L = sqrt(pow(X, 2) + pow(X, 2));
    theta = atan2f(Y, X);
    gamma = (float)acosf((pow(L1, 2) + pow(L, 2) + pow(L2, 2)) / (2 * L2 * L));

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.println("\nDEBUG:: Physical to Abstract:\n");
    Serial.print(buffer);
#endif
}

/**
 * @brief Finds the Leg length, angle, and motor separation for a given X, Y toe point
 *          *This function internally calls the motor angles
 * @return : Returns <Length, Angle, Gamma>
 */
void PhysicalToAbstract(LegConfig leg, float &L, float &theta, float &gamma)
{
    float angle_0, angle_1;

    GetAngles(leg, angle_0, angle_1);

    gamma = (180 - (angle_0 + angle_1)) / 2.0f;
    theta = angle_0 + gamma;
    L = sqrt(pow(L1, 2) + pow(L2, 2) - 2 * L1 * L2 * cosf(gamma));

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.println("\nDEBUG:: Physical to Abstract:\n");
    Serial.print(buffer);
#endif
}

/**
 * @brief Finds the physical (X, Y) position of the toe for a given L and Theta
 *
 * @param L : Leg length
 * @param Theta : Abstract leg angle
 * @return : Pair (X, Y) representing the position of the toe in relation to
 *              the Origin (the hip joint)
 */
void AbstractToPhysical(float L, float Theta, float &x, float &y)
{
    x = L * cosf(Theta);
    y = L * sinf(Theta);

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "X Position: %f, Y Position: %f\n", x, y);
    Serial.println("DEBUG:: Abstract To Physical:");
    Serial.println(buffer);
#endif
}

void MoveToPosition(ODriveArduino &odrive, float t)
{
}

/**
 * @brief Finds the leg position at a given time in a radial movement
 *
 * @param t: time (seconds)
 * @param gait: struct containing radial movement parameters
 * @param X: output toe position
 * @param Y: output Y position
 *
 * @note The function assumes that 2*pi = 1 second
 */
void RadialTrajectory(float t, struct RadialGaitParams gait, float &gamma, float &L, float &X, float &Y)
{
    float theta = gait.theta;
    float a = (gait.L_f - gait.L_i) / 2;
    float b = gait.freq;

    //produces a cosine wave with a midline at the average of Lf and Li
    //with amplitude a and frequency b
    L = a * cosf(2 * PI * b * t) + (gait.L_f + gait.L_i) / 2;
    
    GetGamma(L, theta, gamma);

    AbstractToPhysical(L, theta, X, Y);

// DEBUG OUTPUT
#ifdef DEBUG_HIGH
    char buffer[80];
    sprintf(buffer, "Length: %f, Amp: %f, Freq: %f", L, a, b);
    Serial.println("---------------------------");
    Serial.println("DEBUG:: Radial Trajectory:");
    Serial.println("---------------------------");
    Serial.println(buffer);
#endif
}

void RadialLegMovement(LegConfig leg, float t, struct RadialGaitParams gait, float& theta, float& gamma)
{
    float x;
    float y;
    float L;
    theta = gait.theta;
    RadialTrajectory(t, gait, gamma, L, x, y);

    if (!inBounds(gamma, theta, L))
    {
        leg.EStop();
        return;
    }

    leg.odrv().first.SetCoupledPosition(theta, gamma);
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