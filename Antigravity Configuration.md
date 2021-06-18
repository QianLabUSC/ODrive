### U8II Motor Configuration
Config for the T-Motor Antigravity 4006 Brushless Motor (380kv).

Enter the following commands into the `odrivetool` CLI to configure the motor to work with the ODrive board.

Note: `save_configuration` will cause a reboot, which will cause a temporary disconnect. This is normal.

```
odrv0.axis0.motor.config.current_lim = 15
odrv0.axis0.controller.config.vel_limit = 10
odrv0.axis0.motor.config.calibration_current = 15
odrv0.axis0.motor.config.pole_pairs = 12
odrv0.axis0.motor.config.torque_constant = 0.022
odrv0.axis0.motor.config.motor_type = MOTOR_TYPE_HIGH_CURRENT
odrv0.config.dc_max_negative_current = -1 # prevents the motor from throwing an error when slowing
odrv0.config.max_regen_current = 3
odrv0.config.enable_brake_resistor = True
odrv0.config.brake_resistance = 0.5
odrv0.save_configuration()
```

```
odrv0.axis1.motor.config.current_lim = 15
odrv0.axis1.controller.config.vel_limit = 10
odrv0.axis1.motor.config.calibration_current = 15
odrv0.axis1.motor.config.pole_pairs = 12
odrv0.axis1.motor.config.torque_constant = 0.022
odrv0.axis1.motor.config.motor_type = MOTOR_TYPE_HIGH_CURRENT
odrv0.config.dc_max_negative_current = -1 # prevents the motor from throwing an error when slowing
odrv0.config.max_regen_current = 3
odrv0.config.enable_brake_resistor = True
odrv0.config.brake_resistance = 0.5
odrv0.save_configuration()
```

### AS5047P Encoder Configuration
This assumes the encoder's CSN pin is connected to the ODriveBoard's GPIO7 pin. If you change the wiring, replace the pin number.
```
odrv0.axis0.encoder.config.abs_spi_cs_gpio_pin = 7  # or which ever GPIO pin you choose
odrv0.axis0.encoder.config.mode = ENCODER_MODE_SPI_ABS_AMS
odrv0.axis0.encoder.config.cpr = 2**14
odrv0.save_configuration()
odrv0.reboot()
```

```
odrv0.axis1.encoder.config.abs_spi_cs_gpio_pin = 8  # or which ever GPIO pin you choose
odrv0.axis1.encoder.config.mode = ENCODER_MODE_SPI_ABS_AMS
odrv0.axis1.encoder.config.cpr = 2**14
odrv0.save_configuration()
odrv0.reboot()
```
odrv0.axis1.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
odrv0.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE

### Gain Values
determined by John on 21.06.17
```
odrv0.axis0.controller.config.pos_gain = 51
odrv0.axis0.controller.config.vel_gain = .056
odrv0.axis0.controller.config.vel_integrator_gain = 0.083

odrv0.axis1.controller.config.pos_gain = 51
odrv0.axis1.controller.config.vel_gain = .056
odrv0.axis1.controller.config.vel_integrator_gain = 0.083
```

### Circular Setpoints
```
odrv0.axis0.controller.config.circular_setpoints=True
odrv0.axis1.controller.config.circular_setpoints=True
```