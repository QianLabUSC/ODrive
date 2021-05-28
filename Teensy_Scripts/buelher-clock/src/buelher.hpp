#include <string>
#include <TimeLib.h>

class LegConfig
{
    LegConfig(
        int servoID,
        bool clockwise,
        int zeroing);

    // Motor's Identifier
    // corresponds to `axis` in ODrive system
    int servoID;

    // Whether the motor turns clockwise or counter-clockwise
    bool clockwise;

    // how far to turn to leg to zero it (deg)
    // bounded 0-360
    int zeroing;
};

class Gait
{
    Gait(
        std::string name,
        int right_fore,
        int left_fore,
        int right_hind,
        int left_hind);

    std::string name;
    int right_fore;
    int left_fore;
    int right_hind;
    int left_hind;
};

/**
 * *Buelher Clock
 * Defines the parameters for a 2 phase rotation.
 */
class BuelherClock
{
    BuelherClock(
        float time_slow,
        float time_fast,
        int theta_i,
        int theta_f,
        int damp,
        LegConfig right_fore,
        LegConfig left_fore,
        LegConfig right_hind,
        LegConfig left_hind,
        Gait gait);

    // time spent in the slow phase of rotation
    float time_slow;

    // time spent in the fast phase of rotation
    float time_fast;

    // time to complete one full rotation
    float period();

    // angle to enter slow phase (deg)
    // bounded 0-360
    int theta_i;

    // angle to exit slow phase (deg)
    // bounded 0-360
    int theta_f;

    /**
     * Damping factor on speed adjustment.
     * The smaller the value, the more sensitive the robot is to its angular error.
     * Small values may cause jerky overcorrection.
     * Large values may allow falling out of sync.
     * !Note: this is a holdover from the LSS, and may not be relevant to ODrive motors
     */
    int damp;

    /** Parameters for each of the Robot's 4 legs */
    LegConfig right_fore;
    LegConfig left_fore;
    LegConfig right_hind;
    LegConfig left_hind;

    /** Gait Parameters */
    Gait gait;
};
