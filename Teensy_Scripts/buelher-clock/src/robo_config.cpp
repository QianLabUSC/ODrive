#include "robo_config.hpp"

RoboConfig::RoboConfig(
    LegConfig right_fore,
    LegConfig left_fore,
    LegConfig right_hind,
    LegConfig left_hind) : right_fore(right_fore),
                           left_fore(left_fore),
                           right_hind(right_hind),
                           left_hind(left_hind)
{
}

LegConfig RoboConfig::operator[](const Leg &leg) {
    switch (leg) {
        case Leg::right_fore: return right_fore; break;
        case Leg::left_fore: return left_fore; break;
        case Leg::right_hind: return right_hind; break;
        case Leg::left_hind: return left_hind; break;
    };
    
    // Should never exit without hitting enum!
    exit(1);
}

LegConfig::LegConfig(
    int odrv,
    int axis,
    float init_offset,
    bool invert_direction) : odrv(odrv),
                             axis(axis),
                             init_offset(init_offset),
                             invert_direction(invert_direction)
{
}