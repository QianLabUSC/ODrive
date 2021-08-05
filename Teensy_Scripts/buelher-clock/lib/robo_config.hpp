#include "../lib/ODriveArduino/ODriveArduino.h"

#ifndef ROBO_CONFIG_H
#define ROBO_CONFIG_H
/**
 * Defines settings for a particular motor on an ODrive robot.
 */
class LegConfig
{
public:
    LegConfig(
        int odrv,
        int axis,
        float init_offset,
        bool gyre);

    // Address a specific motor
    // TODO: get actual HardwareSerial and ODriveArduino object!
    int odrv; // which ODrive board is being addressed
    int axis; // which axis (0 or 1) on that board is being addressed

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
    LegConfig right_fore;
    LegConfig left_fore;
    LegConfig right_hind;
    LegConfig left_hind;
};

// - Define Configurations Here:

// 21.08.05 testing config with 2 motors
const RoboConfig TESTING = RoboConfig(
    LegConfig(0, 0, 0.0f, false),
    LegConfig(0, 1, 0.0f, false),
    LegConfig(1, 0, 0.0f, true),
    LegConfig(1, 1, 0.0f, true));

#endif