#include <string>

class Gait
{
public:
    Gait(
        std::string name,
        int right_fore,
        int left_fore,
        int right_hind,
        int left_hind);

    std::string name;
    int right_fore;
    int left_fore;
    int right_hind;
    int left_hind;
};

const Gait TROTTING = Gait(
    "ZERO",
    0,
    180,
    180,
    0);