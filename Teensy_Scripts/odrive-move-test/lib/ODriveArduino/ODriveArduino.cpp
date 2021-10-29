
#include "Arduino.h"
#include "ODriveArduino.h"

static const int kMotorOffsetFloat = 2;
static const int kMotorStrideFloat = 28;
static const int kMotorOffsetInt32 = 0;
static const int kMotorStrideInt32 = 4;
static const int kMotorOffsetBool = 0;
static const int kMotorStrideBool = 4;
static const int kMotorOffsetUint16 = 0;
static const int kMotorStrideUint16 = 2;

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

ODriveArduino::ODriveArduino(Stream& serial)
    : serial_(serial) {}

void ODriveArduino::SetPosition(int motor_number, float position) {
    SetPosition(motor_number, position, 0.0f, 0.0f);
}

void ODriveArduino::SetPosition(int motor_number, float position, float velocity_feedforward) {
    SetPosition(motor_number, position, velocity_feedforward, 0.0f);
}

void ODriveArduino::SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward) {
    serial_ << "p " << motor_number  << " " << position << " " << velocity_feedforward << " " << current_feedforward << "\n";
}

void ODriveArduino::SetVelocity(int motor_number, float velocity) {
    SetVelocity(motor_number, velocity, 0.0f);
}

void ODriveArduino::SetVelocity(int motor_number, float velocity, float current_feedforward) {
    serial_ << "v " << motor_number  << " " << velocity << " " << current_feedforward << "\n";
}

void ODriveArduino::SetCurrent(int motor_number, float current) {
    serial_ << "c " << motor_number << " " << current << "\n";
}

void ODriveArduino::TrapezoidalMove(int motor_number, float position){
    serial_ << "t " << motor_number << " " << position << "\n";
}

float ODriveArduino::readFloat() {
    return readString().toFloat();
}

float ODriveArduino::GetSetpoint(int motor_number) {
    serial_ << "r axis" << motor_number << ".controller.pos_setpoint\n";
    return ODriveArduino::readFloat();
}

float ODriveArduino::GetVelocity(int motor_number){
	serial_<< "r axis" << motor_number << ".encoder.vel_estimate\n";
	return ODriveArduino::readFloat();
}

float ODriveArduino::GetPosition(int motor_number) {
    serial_ << "r axis" << motor_number << ".encoder.pos_circular\n";
    return ODriveArduino::readFloat();
}

float ODriveArduino::GetPosDiff(int motor_number) {
    return ( ODriveArduino::GetSetpoint(motor_number) - ODriveArduino::GetPosition(motor_number) );
}

int32_t ODriveArduino::readInt() {
    return readString().toInt();
}

/**
 * Command a certain desired state for a given axis
 * @param  axis            Axis number (0 or 1)
 * @param  requested_state Desired state, see state ENUM for viable options
 * @param  wait            Block program until ODrive is back to idle?
 * @return                 Did ODrive successfully reach desired state?
 */
bool ODriveArduino::run_state(int axis, int requested_state, bool wait_for_idle, float timeout) {
    int timeout_ctr = (int)(timeout * 10.0f);
    serial_ << "w axis" << axis << ".requested_state " << requested_state << '\n';
    if (wait_for_idle) {
        do {
            delay(100);
            serial_ << "r axis" << axis << ".current_state\n";
        } while (readInt() != AXIS_STATE_IDLE && --timeout_ctr > 0);
    }

    return timeout_ctr > 0;
}

int ODriveArduino::motor_calibrated(int axis) {
    serial_ << "r axis" << axis << ".motor.config.pre_calibrated\n";
    int calibrated = readInt();
    return calibrated;
}

/**
 * @brief Finds the motor angles in Radians from reference of axis0, CCW +
 */
void ODriveArduino::GetThetaGamma(float& theta, float& gamma) {
    // Finds axis 0 and axis 1 motor positions and converts to radians
    float axis_0 = ODriveArduino::GetPosition(0) * 2 * PI;
    float axis_1 = ODriveArduino::GetPosition(1) * 2 * PI + PI; // Transforms axis_1 encoder reading

    theta = (axis_1 - axis_0) / 2;
    gamma = axis_1 - theta;
}

void ODriveArduino::FindMotorAngles(float theta, float gamma, float& axis_0, float& axis_1) {
    axis_0 = theta - gamma;
    axis_1 = theta + gamma - PI;
}

void ODriveArduino::SetCoupledPosition(float theta, float gamma) {
    float axis_0 = theta - gamma;
    float axis_1 = theta + gamma - PI;
    
    ODriveArduino::SetPosition(0, axis_0);
    ODriveArduino::SetPosition(1, axis_1);
}


String ODriveArduino::readString() {
    String str = "";
    static const unsigned long timeout = 1000;
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}

String ODriveArduino::getBoardInfo(){
    serial_ << "i\n";
	String res = readString() + ", " + readString() + ", " + readString() + "\n";
	return res;
}


