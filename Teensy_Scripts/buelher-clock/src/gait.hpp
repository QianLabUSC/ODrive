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

    /**
     * Starting angular offsets for each of the Robot's 4 legs.
     * Assumes each of the legs has been zeroed to a consistent position.
     * Unit: degrees, bounded [0, 360)
     */
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