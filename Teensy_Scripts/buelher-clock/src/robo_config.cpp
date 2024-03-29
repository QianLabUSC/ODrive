#include "robo_config.hpp"

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "TorqueHelpers.hpp"
#include "calibrate.hpp"

RoboConfig::RoboConfig(
    LegConfig right_fore, LegConfig left_fore, LegConfig right_hind,
    LegConfig left_hind,
    std::vector<std::pair<ODriveArduino, HardwareSerial *>> interfaces)
    : interfaces(interfaces),
      right_fore(right_fore),
      left_fore(left_fore),
      right_hind(right_hind),
      left_hind(left_hind),
      legs({right_fore, left_fore, right_hind, left_hind}) {}

void RoboConfig::setup() const {
    /* Serial Start */
    for (auto interface : interfaces) {
        interface.second->begin(BAUD);
    }
    Serial.begin(BAUD);  // Serial to PC
    while (!Serial)
        ;  // wait for Arduino Serial Monitor to open

    /* Confirm ODrive Connection */
    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");
    for (auto interface : interfaces) {
        Serial.println(interface.first.getBoardInfo());
    }

    /* Startup Calibration for ODrive */
    for (LegConfig leg : legs) {
        calibrate(leg.axis(), leg.odrv().first);
        *(leg.odrv().second)
            << "w axis" << leg.axis() << ".controller.input_mode " << 3 << "\n";
        if (checkError(leg.axis(), leg.odrv().first, *(leg.odrv().second)))
            Serial.println("Error in Motor Axis");
    }

    Serial.println(
        "Motor Armed & Ready\n"
        "Command Menu:\n"
        "	'l' -> enter closed loop control.\n"
        "	'b' -> reads bus voltage\n"
        "	'q' -> Sends motors to IDLE STATE\n"
        "	'c' -> Execute Buelher Clock\n");
}

LegConfig RoboConfig::operator[](const Leg &leg) {
    switch (leg) {
        case Leg::right_fore:
            return right_fore;
            break;
        case Leg::left_fore:
            return left_fore;
            break;
        case Leg::right_hind:
            return right_hind;
            break;
        case Leg::left_hind:
            return left_hind;
            break;
    };

    // Should never exit without hitting enum!
    exit(1);
}

LegConfig::LegConfig(HardwareSerial *serial_ptr, int axis, float init_offset,
                     bool invert_direction)
    : _odrv(std::make_pair(ODriveArduino(*serial_ptr), serial_ptr)),
      _axis(axis),
      _init_offset(init_offset),
      invert_direction(invert_direction) {}

void LegConfig::setPosition(float rotations) {
    _odrv.first.SetPosition(_axis, rotations);
}

void LegConfig::setState(ODriveArduino::AxisState_t state) {
    _odrv.first.run_state(_axis, state, false /*don't wait*/);
}
