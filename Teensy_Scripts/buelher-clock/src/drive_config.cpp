#include "drive_config.hpp"
#include "inline_print.hpp"
#include <HardwareSerial.h>
#include <string>

DriveConfig::DriveConfig(
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
    float velocity_integrator_gain)
{
    this->velocity_limit = velocity_limit;
    this->current_limit = current_limit;
    this->pole_pairs = pole_pairs;
    this->torque_constant = torque_constant;
    this->motor_type = motor_type;
    this->abs_spi_cs_gpio_pin = abs_spi_cs_gpio_pin;
    this->mode = mode;
    this->cpr = cpr;
    this->pos_gain = pos_gain;
    this->velocity_gain = velocity_gain;
    this->velocity_integrator_gain = velocity_integrator_gain;
}

void configure(HardwareSerial odrive_serial, int axis, const DriveConfig *config)
{
    odrive_serial << "w axis" << axis << ".controller.config.vel_limit " << config->velocity_limit << '\n';
    odrive_serial << "w axis" << axis << ".motor.config.current_lim " << config->current_limit << '\n';
    odrive_serial << "w axis" << axis << ".motor.config.pole_pairs" << config->pole_pairs << '\n';
    odrive_serial << "w axis" << axis << ".motor.config.torque_constant" << config->torque_constant << '\n';
    odrive_serial << "w axis" << axis << ".motor.config.motor_type" << config->motor_type << '\n';
    odrive_serial << "w axis" << axis << ".encoder.config.abs_spi_cs_gpio_pin" << config->abs_spi_cs_gpio_pin << '\n';
    odrive_serial << "w axis" << axis << ".encoder.config.mode" << config->mode << '\n';
    odrive_serial << "w axis" << axis << ".encoder.config.cpr" << config->cpr << '\n';
    odrive_serial << "w axis" << axis << ".controller.config.pos_gain" << config->pos_gain << '\n';
    odrive_serial << "w axis" << axis << ".controller.config.vel_gain" << config->velocity_gain << '\n';
    odrive_serial << "w axis" << axis << ".controller.config.vel_integrator_gain" << config->velocity_integrator_gain << '\n';
}

