#include "gait.hpp"

using namespace Gait;

Gait::Gait::Gait(std::string name, int right_fore, int left_fore,
                 int right_hind, int left_hind)
    : name(name),
      right_fore(right_fore),
      left_fore(left_fore),
      right_hind(right_hind),
      left_hind(left_hind) {}

int Gait::Gait::operator[](const Leg& leg) {
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

    // Should never be reached!
    return 0;
}