- [1. Five-Bar Leg Operation Guide](#1-five-bar-leg-operation-guide)
  - [1.1. Overview of Operation and Code Library:](#11-overview-of-operation-and-code-library)
  - [1.2. Requirements:](#12-requirements)
  - [1.3. Setup:](#13-setup)
- [2. Code Library Overview](#2-code-library-overview)

# 1. Five-Bar Leg Operation Guide
*PLEASE READ FULL GUIDE PRIOR TO POWERING ON THE TEST STAND*

## 1.1. Overview of Operation and Code Library:
From a high level, the 5-bar leg test system works by connecting your laptop to a Teensy 3.5 board, which in turn interfaces with the ODrive and the motors. To interface with the Teensy 3.5 board, we use PlatformIO, an open-source board support extension that can be added to VSCode.

The code library is organized in three folders: `/include`, `/lib`, and `/src`. `/include` contains header files for functions that are implemented in `/src`. A brief summary of the program files is included below.

## 1.2. Requirements:
1. Install PlatformIO for VSCode

## 1.3. Setup:
1. **NOTE:** If you are unsure of the functionality of your control code, consider disconnecting the lower leg linkages so that the motors cannot drive the leg into an invalid position. This can be done by using a hex wrench to unscrew the bolts in the knee joints. 
2. Connect the Teensy 3.5 to the ODrive board (RX -> TX; TX -> RX; GND -> GND)
3. Connect the Teensy board to your computer with a microUSB cable
4. Connect the power cable from the DC power supply to the ODrive board
5. Turn on the DC Power supply. **Ensure that its voltage is set between 12V and 24V**
6. Ensure that PlatformIO has correctly installed (you should see buttons for Build, Upload, Clean, and Serial Monitor in the bottom left of the screen)
7. **CRITICAL STEP:** Holding just the toe of the leg, prop up the leg so that the knee joints are **ABOVE** the motor axes. The knee joints must be above the motor axis at the start of startup index search.
8. While holding the leg in the position described in step **7**, press `PlatformIO: Upload` to upload the control code to the Teensy 3.5. This will start the calibration sequence for the motor.
9. If motor calibration is successful, the motor will hold a position after finding its index positions. To put the leg into a neutral position, type `e` into the serial monitor. 

# 2. Code Library Overview
`/include` and `/src`:
  - `LegConfig`: Defines the `LegConfig` object that encapsulates the ODrive board, its two motors, serial connections, and its startup procedures.
  - `ModeFacilitator`: This header and source file define routines that help `main.cpp` by initializing program timers and running the different Gaits.
  - `PositionControl`: The Position Control code defines the base kinematic functions that perform conversion between motor angles and the cartesian/polar space. At a higher level, this facilitates the conversion between the Abstract [theta, gamma, length] leg representation and its Physical representation [motor angle 0, motor angle 1]
    - `PositionControl` also defines routines for radial gait movements
  - `SinusoidalGait`: Defines routines for moving the leg in a sinusoidal gait movement with tunable parameters such as stride length, frequency, stride/swing depths. **NOTE: THIS GAIT MOVEMENT IS UNTESTED AND MAY NOT FUNCTION CORRECTLY**

`/lib`:
  - `ODriveArduino`: this contains the low-level ODrive Arduino API that allows for controlling the odrive motors. There are additional functions provided that help other higher level programming elements.





