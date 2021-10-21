#include "PositionControl.hpp"

#include <cmath>

#define DEBUG

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
    L = sqrt(pow(l_1, 2) + pow(l_2, 2) - 2 * l_1 * l_2 * cos(gamma));

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
    x = L * cos(Theta);
    y = L * sin(Theta);

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

void RadialTrajectory(float t, float distance, float angle, float &L, float &theta)
{
}

// ! MUST FIND WORKSPACE EMPIRICALLY
/**
 * @brief Checks whether a current abstract leg position is valid
 */
bool inBounds(float L, float Theta)
{
    return true;
}

/**
 * @brief Checks whether a toe position is in bounds
 */
bool inBounds()
{
    return true;
}