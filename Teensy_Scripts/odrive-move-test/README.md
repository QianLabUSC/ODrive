- [1. TEENSY 3.5 SCRIPTS:](#1-teensy-35-scripts)
  - [1.1. General Notes:](#11-general-notes)
  - [1.2. Repo Structure:](#12-repo-structure)
# 1. TEENSY 3.5 SCRIPTS:
## 1.1. General Notes:
This folder contains a control script for basic configuration/control of the ODrive using a serial monitor input. The code is structured for use with PlatformIO, which connects to the Teensy without the use of the Arduino IDE. Installation instructions for PlatformIO for VSCode can be found at the link: [PlatformIO VSCode Installation](https://platformio.org/install/ide?install=vscode). More information on PlatformIO can be found here: [PlatformIO Getting Started](https://docs.platformio.org/en/latest/what-is-platformio.html). 
## 1.2. Repo Structure:
- The main script is under `src/main.cpp`. 
- The ODrive for Arduino library is under `lib/ODriveArduino`.
- Executing the torque output test will create a Serial Monitor log file, named something like `platformio-device-monitor-##########.log`.
