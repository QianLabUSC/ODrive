#include "../lib/ODriveArduino/ODriveArduino.h"

/**
 * Configuration object reflecting the physical / electrical setup
 * of a 4 legged 1 DOF ODrive Robot.
 */
class RoboConfig
{
public:
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
        int gyre);

    // Address a specific motor
    int odrv; // which ODrive board is being addressed
    int axis; // which axis (0 or 1) on that board is being addressed

    // Initial angular offset the motor should move to to adopt a neutral position.
    // Should be determined experimentally based on the motor's physical placement.
    float init_offset;

    // Indicates whether the motor should rotate normally, or in the opposite direction.
    // -  +1 normally
    // - -1 opposite
    int gyre;
};

// - Define Configurations Here:

// 21.08.05 testing config with 2 motors
const RoboConfig TESTING = RoboConfig(
    LegConfig(0, 0, 0.0f, +1),
    LegConfig(0, 1, 0.0f, +1),
    LegConfig(1, 0, 0.0f, -1),
    LegConfig(1, 1, 0.0f, -1))