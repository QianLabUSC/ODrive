#include "buelher.hpp"
#include <math.h> /* fmod */

BuelherClock::BuelherClock(
    float time_slow,
    float time_fast,
    int theta_i,
    int theta_f,
    int damp,
    Gait gait) : time_slow(time_slow),
                 time_fast(time_fast),
                 theta_i(theta_i),
                 theta_f(theta_f),
                 damp(damp),
                 gait(gait)
{
}

float BuelherClock::period() const
{
    return this->time_fast + this->time_slow;
}

float BuelherClock::d_theta() const
{
    return this->theta_f - this->theta_i;
}

float BuelherClock::omega_slow() const
{
    return this->d_theta() / this->time_slow;
}

float BuelherClock::omega_fast() const
{
    return (360 - this->d_theta()) / this->time_fast;
}

float BuelherClock::time_i() const
{
    return this->theta_i / this->omega_fast();
}

float BuelherClock::time_f() const
{
    return this->time_i() + (this->d_theta() / this->omega_slow());
}

float BuelherClock::getPosition(long elapsed, int wrap = INT32_MAX) const
{
    float s_elapsed = float(elapsed) / 1000.0f;

    // whole number period
    int rotations = int(s_elapsed / period());

    // fractional period
    s_elapsed = fmod(s_elapsed, period());

    // whole rotations
    float angle = rotations * 360.0f;

    // add fractional rotation
    if (s_elapsed <= time_i())
        angle += omega_fast() * s_elapsed;
    else if (s_elapsed <= time_f())
        angle += theta_i + ((s_elapsed - time_i()) * omega_slow());
    else
        angle += theta_f + ((s_elapsed - time_f()) * omega_fast());

    // wrap angle around
    angle = fmod(angle, float(360 * wrap));

    return angle;
}