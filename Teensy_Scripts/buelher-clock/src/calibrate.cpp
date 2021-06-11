#include "../lib/ODriveArduino/ODriveArduino.h"
#include "UtilityHelpers.hpp"

#include "calibrate.hpp"

void calibrate(int axis, ODriveArduino odrive)
{
    const int requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
    Serial << "Axis" << axis << ": Requesting state " << requested_state << '\n';
    odrive.run_state(axis, requested_state, true);
}