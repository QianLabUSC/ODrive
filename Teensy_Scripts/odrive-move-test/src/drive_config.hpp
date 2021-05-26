#include <HardwareSerial.h>

class DriveConfig
{
public:
    DriveConfig(
        float velocity_limit,
        float current_limit,
        int pole_pairs,
        float torque_constant,
        char *motor_type,
        int abs_spi_cs_gpio_pin,
        char *mode,
        int cpr,
        int pos_gain,
        float velocity_gain,
        float velocity_integrator_gain);

    float velocity_limit;
    float current_limit;
    int pole_pairs;
    float torque_constant;
    char *motor_type;
    int abs_spi_cs_gpio_pin;
    char *mode;
    int cpr;

    /**
     * Positional Proportional Gain.
     * Optional Parameter.
     * Determined Empirically by @JohnBush.
     * See: https://docs.odriverobotics.com/control
     */
    int pos_gain;
    float velocity_gain;
    float velocity_integrator_gain;
};

void configure(HardwareSerial odrive_serial, int axis);