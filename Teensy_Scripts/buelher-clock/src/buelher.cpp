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

float BuelherClock::d_theta()
{
    return this->theta_f - this->theta_i;
}

float BuelherClock::omega_slow()
{
    return this->d_theta() / this->time_slow;
}

float BuelherClock::omega_fast()
{
    return (360 - this->d_theta()) / this->time_fast;
}

float BuelherClock::time_i()
{
    return this->theta_i / this->omega_fast();
}

float BuelherClock::time_f()
{
    return this->time_i() + (this->d_theta() / this->omega_slow());
}

LegConfig::LegConfig(
    int servoID,
    bool clockwise,
    int zeroing) : servoID(servoID),
                   clockwise(clockwise),
                   zeroing(zeroing)
{
}

