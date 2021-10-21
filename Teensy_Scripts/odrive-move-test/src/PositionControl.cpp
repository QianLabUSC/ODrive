#include "PositionControl.hpp"

#include <cmath>

#define DEBUG

// !Works off of the assumption that axis 1's angle reference is a mirror of axis 0
// !must change this in case we use an ODrive command to shift axis1's encoder reference
/**
 * @brief Returns a pair of motor angles converted to radians
 */
std::pair<float, float> GetAngles(LegConfig leg)
{
    float angle_0 = leg.odrv().first.GetPosition(0) * 2 * PI;
    float angle_1 = leg.odrv().first.GetPosition(1) * 2 * PI;

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Axis 0: %f, Axis 1: %f\n", angle_0, angle_1);
    Serial.print("DEBUG:: Axis Angle Outputs:\n");
    Serial.print(buffer);
#endif

    return std::make_pair(angle_0, angle_1);
}

/**
 * @brief Converts Abstract leg position into Theta, Gamma values
 *
 * @param L : Leg length
 * @param Theta : Abstract leg angle
 * @return : Pair (Theta, Gamma) representing the motor separation
 *             for the given input parameters
 */
std::pair<float, float> GetGamma(float L, float Theta)
{
    float gamma = acos((pow(l_1, 2) + pow(L, 2) + pow(l_2, 2)) / (2 * l_2 * L));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Theta: %f, Gamma: %f\n", Theta, gamma);
    Serial.print("DEBUG:: Theta Gamma Calculation:\n");
    Serial.print(buffer);
#endif

    return std::make_pair(Theta, gamma);
}

/**
 * @brief Finds the Leg length, angle, and motor separation for a given X, Y toe point
 *
 * @param X : Toe X position
 * @param Y : Toe Y position
 *
 * @return : Returns <Length, Angle, Gamma>
 */
std::tuple<float, float, float> PhysicalToAbstract(float X, float Y)
{
    float L = sqrt(pow(X, 2) + pow(X, 2));
    float theta = atan2(Y, X);
    float gamma = (float)acos((pow(l_1, 2) + pow(L, 2) + pow(l_2, 2)) / (2 * l_2 * L));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.print("DEBUG:: Physical to Abstract:\n");
    Serial.print(buffer);
#endif

    return std::make_tuple(L, theta, gamma);
}

/**
 * @brief Finds the Leg length, angle, and motor separation for a given X, Y toe point
 *          *This function internally calls the motor angles
 * @return : Returns <Length, Angle, Gamma>
 */
std::tuple<float, float, float> PhysicalToAbstract(LegConfig leg)
{
    std::pair<float, float> angle = GetAngles(leg);

    float gamma = (180 - (angle.first + angle.second)) / 2.0f;
    float theta = angle.first + gamma;
    float L = sqrt(pow(l_1, 2) + pow(l_2, 2) - 2 * l_1 * l_2 * cos(gamma));

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "Length: %f, Theta: %f, Gamma: %f\n", L, theta, gamma);
    Serial.print("DEBUG:: Physical to Abstract:\n");
    Serial.print(buffer);
#endif

    return std::tuple<float, float, float>(L, theta, gamma);
}

/**
 * @brief Finds the physical (X, Y) position of the toe for a given L and Theta
 *
 * @param L : Leg length
 * @param Theta : Abstract leg angle
 * @return : Pair (X, Y) representing the position of the toe in relation to
 *              the Origin (the hip joint)
 */
std::pair<float, float> AbstractToPhysical(float L, float Theta)
{
    float x = L * cos(Theta);
    float y = L * sin(Theta);

// DEBUG OUTPUT
#ifdef DEBUG
    char buffer[80];
    sprintf(buffer, "X Position: %f, Y Position: %f\n", x, y);
    Serial.print("DEBUG:: Abstract To Physical:\n");
    Serial.print(buffer);
#endif

    return std::pair<float, float>(x, y);
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