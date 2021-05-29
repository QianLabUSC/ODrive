#include "gait.hpp"

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