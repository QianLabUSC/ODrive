#pragma once

#include <vector>
#include <utility>
#include <stdio.h>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "LegConfig.hpp"

// Global Debug Flag
const float l_1 = 0.1;
const float l_2 = 0.2;

std::pair<float, float> GetAngles(LegConfig leg);

std::pair<float, float> GetGamma(float L, float Theta);

std::tuple<float, float, float> PhysicalToAbstract(float X, float Y);

std::tuple<float, float, float> PhysicalToAbstract(LegConfig leg);

std::pair<float, float> AbstractToPhysical(float L, float Theta);



bool inBounds(float L, float Theta);

bool inBounds();
