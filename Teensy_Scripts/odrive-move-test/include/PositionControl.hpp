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

void PhysicalToAbstract(LegConfig leg, float& L, float& theta, float& gamma);

void AbstractToPhysical(float L, float Theta, float& x, float& y);

void MoveToPosition(LegConfig leg, float t);

void RadialTrajectory(float t, float distance, float angle, float& L, float& theta);

bool inBounds(float L, float Theta);

bool inBounds();
