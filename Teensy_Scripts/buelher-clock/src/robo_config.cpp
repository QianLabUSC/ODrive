#include "robo_config.hpp"
LegConfig::LegConfig(
    ODriveArduino odrv,
    int axis,
    float init_offset,
    int gyre) : odrv(odrv),
                axis(axis),
                init_offset(init_offset),
                gyre(gyre) {}