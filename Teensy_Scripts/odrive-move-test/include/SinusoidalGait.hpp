#include "PositionControl.hpp"

struct GaitParams {
    float stance_height; // leg extension 
    float stride_length; //angle of step
    float freq;
    float stance_ratio;
    float stance_depth;
    float swing_retraction;
};

extern struct GaitParams Gaits[6];

void SinusoidalTrajectory(float t, struct GaitParams gait, float &gamma, float &L, float &X, float &Y);

void GaitMovement(LegConfig Leg, float t, struct GaitParams gait, float& theta, float& gamma);

bool ValidateGaitParams(struct GaitParams gait);

bool inBounds(float Gamma, float Theta, float L);

bool inBounds(float x, float y);