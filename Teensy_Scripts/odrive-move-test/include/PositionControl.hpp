#pragma once

#include <tuple>
#include <utility>
#include <stdio.h>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "LegConfig.hpp"

// Global Debug Flag
const float l_1 = 0.1;
const float l_2 = 0.2;

void GetAngles(LegConfig leg, float& angle_0, float& angle_1);

void GetGamma(float L, float theta, float& gamma);

void PhysicalToAbstract(float X, float Y, float& L, float& theta, float& gamma);

void PhysicalToAbstract(float X, float Y, float &theta, float &gamma);

void PhysicalToAbstract(LegConfig leg, float& L, float& theta, float& gamma);

void AbstractToPhysical(float L, float Theta, float& x, float& y);

void MoveToPosition(LegConfig leg, float t);

void RadialTrajectory(float t, struct RadialGaitParams gait, float& X, float& Y);

void RadialLegMovement(LegConfig leg, float t, struct RadialGaitParams gait, float& theta, float& gamma);

bool inBounds(float Gamma, float Theta, float L);

bool inBounds(float x, float y);

struct RadialGaitParams {
    float L_i = 0.1f; // Initial Length of leg
    float L_f = 0.2f; // Final leg length
    float theta = PI/2; // Angle of radial movement
    float freq = 0.5f; // Frequency of one movement cycle (Hz)
};

extern struct RadialGaitParams radial_gait_params[6];