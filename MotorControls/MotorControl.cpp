#include "MotorControl.h"
#include <math.h>

using namespace std;

MotorControl::MotorControl(float leg1, float leg2, float leg3, float leg4, float leg5){
    l1 = leg1;
    l2 = leg2;
    l3 = leg3;
    l4 = leg4;
    l6 = leg5;
}

/**
 * @param x: x-coordinate of extended leg end
 * @param y: y-coordinate of extended leg end
 * 
 * @return pair of floats representing the angle from positive x-axis to each of the two upper leg lengths(radians)
 * 
 * @brief takes the point of the extended toe and calculates the two angles relative to the horizontal; reverse kinematics
 */
std::pair<float, float> MotorControl::XYtoTheta(float x, float y)
{
    float l16 = sqrt(pow(x, 2) + pow(y, 2));
    float theta_h = acos(-1 * x / l16); //positive angle as defined by axis
    float l46 = l3 + l6;
    float cVal = (l16 * l16 + l4 * l4 - l46 * l46) / (2 * l16 * l4); //calculating cos values
    if(cVal > 1){ //ensuring cos values do not exceed thresholds due to floating-point errors
        cVal = 1;
    }
    if(cVal < -1){
        cVal = -1;
    }
    float theta_46 = acos(cVal);
    float theta_1 = theta_h - theta_46;
    std::pair<float, float> p4 = std::make_pair(l4 * cos(theta_1), l4 * sin(theta_1));
    float theta_6 = acos((p4.first - x) / l46); //negative angle as defined by axis, but using it as positive
    std::pair<float, float> p3 = std::make_pair(x + l3 * cos(theta_6), y - l3 * sin(theta_6));
    float l13 = sqrt(pow(p3.first, 2) + pow(p3.second, 2));
    float theta_b = acos((l4 * l4 + l13 * l13 - l3 * l3) / (2 * l4 * l13));
    float theta_2 = theta_1 - 2 * theta_b;
    return std::make_pair(theta_1, theta_2);
}

/**
 * @param theta_1: the acute angle between the positive horizontal and the upper-front leg (radians)
 * @param theta_2: the acute angle between the positive horizontal and the upper-back leg (radians)
 * 
 * @return pair of floats representing the cartesian coordinates of the end of the extended toe
 * 
 * @brief takes two angles and calculates the location of the extended toe; forward kinematics
 */
std::pair<float, float> MotorControl::ThetatoXY(float theta_1, float theta_2)
{
    if(theta_1 > M_PI)
        theta_1 -= M_PI;
    if (theta_2 > M_PI)
        theta_2 = 2 * M_PI - theta_2;
    float theta_n = 180 - theta_2;
    std::pair<float, float> p2 = std::make_pair(-1 * l1 * cos(theta_n), l1 * sin(theta_n)); //points arranged counter-clock-wise from top left
    std::pair<float, float> p4 = std::make_pair(l4 * cos(theta_1), l4 * sin(theta_1));
    float c = sqrt(pow(p4.first - p2.first,2) + pow(p4.second - p2.second,2)); //distance formula
    float theta_3 = acos((c * c + l2 * l2 - l3 * l3) / (2 * c * l2)); //law of cosines
    float theta_4 = acos((c * c + l3 * l3 - l2 * l2) / (2 * c * l3)); //law of cosines
    float theta_h = acos((p4.first - p2.first)/c);
    std::pair<float, float> p3 = std::make_pair(p2.first + l2 * cos(theta_3 + theta_h), p2.second + l2 * sin(theta_3 + theta_h)); //bottom corner of parallelogram
    std::pair<float, float> p6 = std::make_pair(p3.first - l6 * cos(theta_4 - theta_h), p3.second + l6 * sin(theta_4 - theta_h)); //extended toe point
    return p6;
}

/**
 * @param theta_1: the acute angle between the positive horizontal and the upper-front leg (radians)
 * @param theta_2: the acute angle between the positive horizontal and the upper-back leg (radians)
 * 
 * @return pair of floats representing the radius and angle from the positive horizontal to the intersection of the lower legs
 * 
 * @brief takes two angles and calculates the polar coordinates of the intersection of the lower legs; forward kinematics
 */
std::pair<float, float> MotorControl::ThetatoPolar(float theta_1, float theta_2){
    float angle = (theta_2 + theta_1) / 2;
    std::pair<float, float> p2 = std::make_pair(-1 * l1 * cos(theta_1), l1 * sin(theta_1)); //points arranged counter-clock-wise from top left
    std::pair<float, float> p4 = std::make_pair(l4 * cos(theta_2), l4 * sin(theta_2));
    float c = sqrt(pow(p4.first - p2.first, 2) + pow(p4.second - p2.second, 2)); //distance formula
    float theta_h = acos((p4.first - p2.first) / c);
    float theta_R = 2 * (theta_1 - theta_h);
    float r = sqrt(l4 * l4 + l3 * l3 - 2 * l4 * l3 * cos(theta_R)); //law of cosines
    return std::make_pair(r, angle);
}

/**
 * @param r: length from origin to intersection of lower legs
 * @param angle: angle from positive x-axis to intersection of lower legs
 * 
 * @return pair of floats representing the angle from positive x-axis to each of the two upper leg lengths(radians)
 * 
 * @brief given the length and angle from the origin to the intesection of lower legs, finds both upper leg angle relative to positive x-axis; reverse kinematics
 */
std::pair<float, float> MotorControl::PolartoTheta(float r, float angle){
    float cVal = (r * r + l4 * l4 - l3 * l3) / (2 * r * l4);
    if (cVal > 1){ //ensuring cos values do not exceed thresholds due to floating-point errors
        cVal = 1;
    }
    if (cVal < -1){
        cVal = -1;
    }
    float theta_3 = acos(cVal);
    float theta_1 = angle - theta_3;
    float theta_2 = angle + theta_3;
    return std::make_pair(theta_1, theta_2);
}

/**
 * @param x: x-coordinate of a point
 * @param y: y-coordinate of a point
 * 
 * @return pair of floats representing the polar coordinates of a specified point
 * 
 * @brief helper function converting a point from cartesian to polar coordinates
 */
std::pair<float, float> MotorControl::CartesianToPolar (float x, float y){
    float r = sqrt(x * x + y * y);
    float theta = acos(y/x);
    return std::make_pair(r, theta);
}

/**
 * @param r: radius from origin to a point
 * @param theta: angle (radians) from positive-x axis to point
 * 
 * @return pair of floats representing the cartesian coordinates of a specified point
 * 
 * @brief helper function converting a point from polar to cartesian coordinates
 */
std::pair<float, float> MotorControl::PolarToCartesian(float r, float theta){
    float x = r * cos(theta);
    float y = r * sin(theta);
    return std::make_pair(x, y);
}