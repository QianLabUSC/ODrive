#include <string>
#include <TimeLib.h>

#include "gait.hpp"

class BuelherClock
{
public:
    BuelherClock(
        float time_slow,
        float time_fast,
        int theta_i,
        int theta_f,
        int damp,
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

    /** Gait Parameters */
    Gait gait;

    /* COMPUTED PARAMETERS */

    // time to complete one full rotation
    float period() const;

    // Angular size of the slow region
    // - Unit: degrees
    float d_theta() const;

    // Target angular speed during slow phase.
    // - Unit: degrees / second
    float omega_slow() const;

    // Target angular speed during fast phase.
    // - Unit: degrees / second
    float omega_fast() const;

    // Time in period to enter slow phase
    // - Unit: seconds
    float time_i() const;

    // Time in period to exit slow phase
    // - Unit: seconds
    float time_f() const;

    /* OTHER FUNCTIONS */
    float getPosition(long elapsed, int wrap = INT32_MAX) const;
};

const BuelherClock EXAMPLE = BuelherClock(
    2.0,       // time_slow,
    2.0,       // time_fast,
    60,        // theta_i,
    120,       // theta_f,
    30,        // damp,
    TROTTING);