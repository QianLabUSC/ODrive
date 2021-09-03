### U8II Motor Configuration

Enter the following commands into the `odrivetool` CLI to configure the U8II motor to work with the ODrive board.

Note: `save_configuration` will cause a reboot, which will cause a temporary disconnect. This is normal.

```
odrv0.axis0.motor.config.current_lim = 15
odrv0.axis0.controller.config.vel_limit = 10
odrv0.axis0.motor.config.calibration_current = 15
odrv0.axis0.motor.config.pole_pairs = 21
odrv0.axis0.motor.config.torque_constant = 0.055
odrv0.axis0.motor.config.motor_type = MOTOR_TYPE_HIGH_CURRENT
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