#include "PositionControl.hpp"

struct GaitParams {
    //* User Defined Gait Parameters
    float stance_height; // leg extension 
    float stride_length; //angle of step
    float freq; // Step frequency in Hz
    float stance_ratio; // Percent of time foot is on ground
    float stance_depth; // Depth of step during stance phase
    float swing_retraction; // Retraction distance during swing
    
    
    //* ----NOTE----
    //* Leave these variables as NAN. They are calculated in the program.
    float stance_freq;
    float swing_freq;
    float x_amp;
};

extern struct GaitParams Gaits[6];

void SinusoidalTrajectory(float t, struct GaitParams gait, float& theta, float &gamma, float &L, float &X, float &Y);

void GaitMovement(LegConfig Leg, float t, struct GaitParams gait, float& theta, float& gamma);

//* Call this function before starting a movement
bool ValidateGaitParams(struct GaitParams gait);

bool inBounds(float Gamma, float Theta, float L);

bool inBounds(float x, float y);