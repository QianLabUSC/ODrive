#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <iostream>

class MotorControl {

public:
    MotorControl(float leg1, float leg2, float leg3, float leg4, float leg6);
    std::pair<float, float> XYtoTheta(float x, float y);
    std::pair<float, float> ThetatoXY(float theta_1, float theta_2);
    std::pair<float, float> ThetatoPolar(float theta_1, float theta_2);
    std::pair<float, float> PolartoTheta(float r, float angle);

private:
    std::pair<float, float> CartesianToPolar(float x, float y);
    std::pair<float, float> PolarToCartesian(float r, float theta);

    float l1, l2, l3, l4, l6;
};
#endif