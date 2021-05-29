#include <string>
#include <TimeLib.h>

#include "gait.hpp"

class LegConfig
{
public:
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

/**
 * *Buelher Clock
 * Defines the parameters for a 2 phase rotation.
 */
class BuelherClock
{
public:
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

    // time to complete one full rotation
    float period();

    // Angular size of the slow region
    // - Unit: degrees
    float d_theta();

    // Target angular speed during slow phase.
    // - Unit: degrees / second
    float omega_slow();

    // Target angular speed during fast phase.
    // - Unit: degrees / second
    float omega_fast();

    // Time in period to enter slow phase
    // - Unit: seconds
    float time_i();

    // Time in period to exit slow phase
    // - Unit: seconds
    float time_f();
};

const BuelherClock EXAMPLE = BuelherClock(
    2.0,       // time_slow,
    2.0,       // time_fast,
    60,        // theta_i,
    120,       // theta_f,
    30,        // damp,
    LegConfig( // right_fore
        101,   // servoID,
        true,  // clockwise,
        37),   // zeroing,
    LegConfig( // left_fore
        241,   // servoID
        false, // clockwise
        38),   // zeroing
    LegConfig( // right_hind
        42,    // servoID
        true,  // clockwise
        312),  // zeroing
    LegConfig( // left_hind
        69,    // servoID
        false, // clockwise
        88),   // zeroing
    TROTTING);