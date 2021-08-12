#include <HardwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"

#include "legs.hpp"

#ifndef ROBO_CONFIG_H
#define ROBO_CONFIG_H
/**
 * Defines settings for a particular motor on an ODrive robot.
 * !Note: This class reflects a physical / electrical setup, and therefore is highly subject to change.
 * This source code may change a lot!
 */
class LegConfig
{
public:
    LegConfig(
        HardwareSerial odrv,
        int axis,
        float init_offset,
        bool gyre);

    // Address a specific motor
    // TODO: get actual HardwareSerial and ODriveArduino object!
    HardwareSerial odrv; // which ODrive board is being addressed
    int axis;            // which axis (0 or 1) on that board is being addressed

    /**
     * Initial angular offset for a neutral / zero position.
     * Determine experimentally.
     * @units: rotations
     * @bounds: [0, 1]
     */
    float init_offset;

    // Indicates whether the motor should rotate normally, or in the opposite direction.
    bool invert_direction;
};

/**
 * Configuration object reflecting the physical / electrical setup
 * of a 4 legged 1 DOF ODrive Robot.
 */
class RoboConfig
{
public:
    static const int MOTOR_COUNT = 4;

    RoboConfig(
        LegConfig right_fore,
        LegConfig left_fore,
        LegConfig right_hind,
        LegConfig left_hind);

    // Option to look up a leg using its enum.
    // !Note: Read Only!
    LegConfig operator[](const Leg &leg);

    void run_config();

    LegConfig right_fore;
    LegConfig left_fore;
    LegConfig right_hind;
    LegConfig left_hind;
};

// !Note: Define Configurations Here:

// 21.08.05 testing config with 2 motors
const RoboConfig TESTING = RoboConfig(
    LegConfig(Serial1, 0, 0.0f, false), // right_fore
    LegConfig(Serial1, 1, 0.0f, true),  // left_fore
    LegConfig(Serial2, 0, 0.0f, false), // right_hind
    LegConfig(Serial2, 1, 0.0f, true)   // left_hind
);

#endif