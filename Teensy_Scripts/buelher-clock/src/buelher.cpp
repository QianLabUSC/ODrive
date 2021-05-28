#include "buelher.hpp"

BuelherClock::BuelherClock(
    float time_slow,
    float time_fast,
    int theta_i,
    int theta_f,
    int damp,
    LegConfig right_fore,
    LegConfig left_fore,
    LegConfig right_hind,
    LegConfig left_hind,
    Gait gait) : time_slow(time_slow),
                 time_fast(time_fast),
                 theta_i(theta_i),
                 theta_f(theta_f),
                 damp(damp),
                 right_fore(right_fore),
                 left_fore(left_fore),
                 right_hind(right_hind),
                 left_hind(left_hind),
                 gait(gait)
{
}

float BuelherClock::period()
{
    return this->time_fast + this->time_slow;
}

LegConfig::LegConfig(
    int servoID,
    bool clockwise,
    int zeroing) : servoID(servoID),
                   clockwise(clockwise),
                   zeroing(zeroing)
{
}

Gait::Gait(
    std::string name,
    int right_fore,
    int left_fore,
    int right_hind,
    int left_hind) : name(name),
                     right_fore(right_fore),
                     left_fore(left_fore),
                     right_hind(right_hind),
                     left_hind(left_hind)
{
}