#include <HardwareSerial.h>
#include <string>

class DriveConfig
{
public:
    DriveConfig(
        float velocity_limit,
        float current_limit,
        int pole_pairs,
        float torque_constant,
        String motor_type,
        int abs_spi_cs_gpio_pin,
        String mode,
        int cpr,
        int pos_gain,
        float velocity_gain,
        float velocity_integrator_gain);

    float velocity_limit;
    float current_limit;
    int pole_pairs;
    float torque_constant;
    String motor_type;
    int abs_spi_cs_gpio_pin;
    String mode;
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

void configure(HardwareSerial odrive_serial, int axis, const DriveConfig *config);

/**
 * *21.05.26 John Bush's current ODrive Motor.
 * Configured for the T-Motor U8II KV150 w/ AS5047P rotary encoder.
 */
const DriveConfig TEST_CONFIG = DriveConfig(
    15.0f,                      // vel_limit
    15.0f,                      // current_lim
    21,                         // pole_pairs
    0.061f,                     // torque_constant
    "MOTOR_TYPE_HIGH_CURRENT",  // motor_type
    7,                          // abs_spi_cs_gpio_pin
    "ENCODER_MODE_SPI_ABS_AMS", // mode
    16384,                      // cpr
    150,                        // pos_gain
    0.3f,                       // vel_gain
    1.5                         // vel_integrator_gain
);