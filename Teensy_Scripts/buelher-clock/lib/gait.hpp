#include <string>

#include "legs.hpp"

#ifndef __GAIT__
#define __GAIT__

/**
 * Configuration object describing a gait in a 4 legged 1 DOF robot.
 */
class Gait {
   public:
    Gait(std::string name, int right_fore, int left_fore, int right_hind,
         int left_hind);

    // Option to look up offsets by leg enum.
    // !Note: Read Only!
    int operator[](const Leg &leg);

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

const Gait TROTTING = Gait("TROT",
                           0,    // right_fore
                           180,  // left_fore
                           180,  // right_hind
                           0);   // left_hind

const Gait BOUNDING = Gait("BOUND",
                           0,     // right_fore
                           0,     // left_fore
                           180,   // right_hind
                           180);  // left_hind
#endif
