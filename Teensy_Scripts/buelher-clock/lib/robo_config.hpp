#include <HardwareSerial.h>

#include <utility>
#include <vector>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "legs.hpp"

#ifndef ROBO_CONFIG_H
#define ROBO_CONFIG_H
/**
 * Defines settings for a particular motor on an ODrive robot.
 * !Note: This class reflects a physical / electrical setup,
 * and therefore is highly subject to change.
 *
 * This source code may change a lot!
 */
class LegConfig {
   public:
    LegConfig(HardwareSerial odrv, int axis, float init_offset, bool gyre);

    // Address a specific motor
    // TODO: get actual HardwareSerial and ODriveArduino object!
    HardwareSerial odrv;  // which ODrive board is being addressed
    int axis;  // which axis (0 or 1) on that board is being addressed

    /**
     * Initial angular offset for a neutral / zero position.
     * Determine experimentally.
     * @units: rotations
     * @bounds: [0, 1]
     */
    float init_offset;

    // Indicates whether the motor should rotate normally, or in the opposite
    // direction.
    bool invert_direction;
};

/**
 * Configuration object reflecting the physical / electrical setup
 * of a 4 legged 1 DOF ODrive Robot.
 * Assumptions: we are using Serial 1 and Serial 2 on the Teensy Arduino.
 */
class RoboConfig {
   public:
    static const int MOTOR_COUNT = 4;

    RoboConfig(
        LegConfig right_fore, LegConfig left_fore, LegConfig right_hind,
        LegConfig left_hind,
        std::vector<std::pair<ODriveArduino, HardwareSerial>> interfaces);

    // Option to look up a leg using its enum.
    // - Read Only!
    LegConfig operator[](const Leg &leg);

    void run_config();

    // The serial interfaces to the legs.
    // Programmer must ensure these are the same ODriveArduinos and Serials
    // passed to the legs.
    std::vector<std::pair<ODriveArduino, HardwareSerial>> interfaces;

    LegConfig right_fore;
    LegConfig left_fore;
    LegConfig right_hind;
    LegConfig left_hind;
};

#endif