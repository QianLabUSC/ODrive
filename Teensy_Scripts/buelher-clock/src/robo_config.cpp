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

LegConfig::LegConfig(
    int odrv,
    int axis,
    float init_offset,
    int gyre) : odrv(odrv),
                axis(axis),
                init_offset(init_offset),
                gyre(gyre)
{
}