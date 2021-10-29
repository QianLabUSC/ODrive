#include "PositionControl.hpp"

#include <cmath>

#define DEBUG

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
    {0.04f, .20f, PI/2, .5f},
    {NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN},
    {NAN, NAN, NAN, NAN},
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
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Axis 0: %f, Axis 1: %f\n", angle_0, angle_1);
    Serial.print("DEBUG:: Axis Angle Outputs:\n");
    Serial.print(buffer);
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
    gamma = acos((pow(l_1, 2) + pow(L, 2) + pow(l_2, 2)) / (2 * l_2 * L));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Theta: %f, Gamma: %f\n", theta, gamma);
    Serial.print("DEBUG:: Theta Gamma Calculation:\n");
    Serial.print(buffer);
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
    gamma = (float)acos((pow(l_1, 2) + pow(L, 2) + pow(l_2, 2)) / (2 * l_2 * L));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.print("DEBUG:: Physical to Abstract:\n");
    Serial.print(buffer);
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
    gamma = (float)acosf((pow(l_1, 2) + pow(L, 2) + pow(l_2, 2)) / (2 * l_2 * L));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.print("DEBUG:: Physical to Abstract:\n");
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
    L = sqrt(pow(l_1, 2) + pow(l_2, 2) - 2 * l_1 * l_2 * cosf(gamma));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.print("DEBUG:: Physical to Abstract:\n");
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
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "X Position: %f, Y Position: %f\n", x, y);
    Serial.print("DEBUG:: Abstract To Physical:\n");
    Serial.print(buffer);
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
void RadialTrajectory(float t, struct RadialGaitParams gait, float &X, float &Y)
{
    float theta = gait.theta;
    float a = gait.L_f - gait.L_i;
    float b = gait.freq;

    float L = a * cosf(2 * PI * b * t) + a / 2;

    AbstractToPhysical(L, theta, X, Y);
}

void RadialLegMovement(LegConfig leg, float t, struct RadialGaitParams gait, float& theta, float& gamma)
{
    float x;
    float y;

    RadialTrajectory(t, gait, x, y);
    PhysicalToAbstract(x, y, theta, gamma);

    if (!inBounds(x, y))
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
    if (Gamma <= 0.087 || Gamma > 2.61 || 
        Theta < -2.47 || Theta > 2.47 ||
        L <= 0.03 || L >= 0.3) { // ! CALCULATE PRECISE L RANGE FROM GAMMA
        return false;
    } else {
        return true; 
    }
    
}

/**
 * @brief Checks whether a toe position is in bounds
 */
bool inBounds(float x, float y)
{
    float L = sqrtf(pow(x, 2) + pow(y, 2));
    
    if (L <= 0.03 || L >= 0.3) { // ! CALCULATE PRECISE L RANGE FROM GAMMA
        return false;
    } else {
        return true;
    }
    
}