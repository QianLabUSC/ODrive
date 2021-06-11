- [1. ODRIVE BASIC SETUP:](#1-odrive-basic-setup)
  - [1.1. Introduction:](#11-introduction)
  - [1.2. Parts List:](#12-parts-list)
    - [1.2.1. ODrive Parts:](#121-odrive-parts)
    - [1.2.2. Per motor parts:](#122-per-motor-parts)
    - [1.2.3. Other Parts/Tools:](#123-other-partstools)
- [2. Assembly](#2-assembly)
  - [2.1. AS5047P Encoder Assembly](#21-as5047p-encoder-assembly)
    - [2.1.1. Encoder Resistor Configuration](#211-encoder-resistor-configuration)
    - [2.1.2. Encoder Voltage Selector](#212-encoder-voltage-selector)
    - [2.1.3. Encoder Header Pin](#213-encoder-header-pin)
  - [2.2. AS5048A Encoder Assembly](#22-as5048a-encoder-assembly)
    - [2.2.1. Header Pin Installation](#221-header-pin-installation)
  - [2.3. Encoder Magnet Assembly](#23-encoder-magnet-assembly)
  - [2.4. Mounting the Encoder](#24-mounting-the-encoder)
  - [2.5. Putting it All Together](#25-putting-it-all-together)
- [3. Setting up the ODrive Board and connecting motors and encoders](#3-setting-up-the-odrive-board-and-connecting-motors-and-encoders)
    - [3.0.1. Powering the Odrive](#301-powering-the-odrive)
    - [3.0.2. Wiring the Encoder and Motor](#302-wiring-the-encoder-and-motor)
- [4. Anaconda/Odrivetool Setup and Config](#4-anacondaodrivetool-setup-and-config)
  - [4.1. Downloading and Installing Tools](#41-downloading-and-installing-tools)
    - [4.1.1. Windows](#411-windows)
    - [4.1.2. OSX](#412-osx)
  - [4.2. Start `odrivetool`](#42-start-odrivetool)
- [5. ODrive Calibration](#5-odrive-calibration)
  - [5.1. Motor Configuration](#51-motor-configuration)
  - [5.2. Encoder Calibration](#52-encoder-calibration)
  - [5.3. Motor Calibration and Startup](#53-motor-calibration-and-startup)
  - [5.4. Motor Tuning](#54-motor-tuning)
- [6. Controlling the ODrive with a Teensy Microcontroller (or Equivalent)](#6-controlling-the-odrive-with-a-teensy-microcontroller-or-equivalent)
  - [Downloading PlatformIO in VSCode to Run Scripts](#downloading-platformio-in-vscode-to-run-scripts)
  
# 1. ODRIVE BASIC SETUP:
## 1.1. Introduction:
ODrive allows for precise control of up to 2 BLDC motors and 2 rotary encoders. The ODrive can be interfaced with through a microcontroller such as an Arduino or Teensy board to run control loops. For any questions regarding setup, contact John Bush (johncbus@usc.edu) or through slack message
## 1.2. Parts List:
### 1.2.1. ODrive Parts:
- [ ] 3x heat sinks per motor installed
- [ ] (if not already on the ODrive board) 2x 3 terminal block connectors
- [ ] 10-12AWG wire for power cable
- [ ] 2x banana plugs
- [ ] 1x Female pin header 
- [ ] *OPTIONAL:* 2x XT60 connector pairs
### 1.2.2. Per motor parts:
- [ ] 1x U8II KV150 BLDC motor (or similar motor)
- [ ] 1x AS5047P **OR** AS5048A rotary encoder board and included magnet (AS5047P-TS_EK_AB or AS5048A-TS_EK_AB)
- [ ] 1x (for AS5047P ONLY) 0 ohm 0603 package resistor (if you fry the one included on the encoder)
- [ ] 1x Encoder mounting plate (redesigned plate in progress)
- [ ] 1x motor mount
- [ ] 4x M2.5 x 8mm socket head screws
- [ ] 4x M2.5 Thin type nuts
- [ ] 1x M4 x 10mm screw
- [ ] 1x M4 x 14mm screw
- [ ] 2x M4 x 18mm screws
- [ ] 4x M4 x 30mm screws (can be other length, just to stand off the motor from the ground)
- [ ] 7x M4 nuts 
### 1.2.3. Other Parts/Tools:
- [ ] Soldering iron
- [ ] fine solder
- [ ] assorted jumper cables
- [ ] header pins
- [ ] super glue
- [ ] wire stripper
- [ ] Allen keys/hex wrenches
- [ ] file/sandpaper

# 2. Assembly
**NOTES:**

- **THE AS5048A ENCODER IS RECOMMENDED**
- **if you need help soldering, check out [this tutorial](https://www.makerspaces.com/how-to-solder/)**

## 2.1. AS5047P Encoder Assembly
**NOTE: "front" refers to the side with pin labels and the actual encoder chip, while "back" refers to the side with the serial number sticker.**
### 2.1.1. Encoder Resistor Configuration
1. The AS5047P encoder package is made to work in either a 3.3V or 5V (default) power configuration, and for our purposes we need to select the 3.3V operation setting. On the eval board, notice the 0 ohm 0603 package resistor at R1 on the front of the board. 
   1. First, desolder the resistor at R1
   2. Solder the 0 ohm resistor to R2. *(if damaged, replace the resistor)*
### 2.1.2. Encoder Voltage Selector
1. Next we need to set the voltage selector at JP1.
   1. Desolder and remove the header pins in JP1, since they will otherwise interfere with the motor  
   2. Next either:
      1. Resolder the JP1 header pins so that they protrude from the **back** of the board and then connect the 5V and 3.3V pins or,
      2. Solder a jumper wire between [5V] and [3.3V] on the **back** of the board.
### 2.1.3. Encoder Header Pin
1. Solder the 2x8 pin header so that it protrudes from the **back** of the board.
## 2.2. AS5048A Encoder Assembly
### 2.2.1. Header Pin Installation
1. The AS5048A/B Encoder has a 1x8 pinout, but we don't use the 5V pin. Solder a 1x7 right angle header into pin holes 3.3V through GND on the encoder board.
   1. Solder the 1x7 pin header so that it protrudes from the **back** of the board.
2. For 3V operation necessary for the ODrive, we must short the 3.3V and 5V pins of the board. To do this, solder a short piece of wire between the 3.3V pin and the 5V contact. 
3. **It is crucial that the header is soldered in correctly for calibration to succeed. Check that there are no cold solder joints**
## 2.3. Encoder Magnet Assembly
<br><img src="/images/magnet_screw.jpg" alt="diagram: screw with spacer nuts"
title="diagram: screw with spacer nuts" width="300" height="200" />
<img src="/images/magnet_assembly.jpg" alt="diagram: magnet and nut assembly"
title="diagram: magnet and nut assembly" width="300" height="200" />

1. Take an M4 x 14mm screw and screw 2 nuts fully onto it to act as spacers.
2. Use the magnet alignment jig to glue the magnet onto the center of an M4 nut 
3. Remove the plastic dust cover from the bottom of the U8II motor.
4. From the top side of the motor, drop the M4 screw assembly into the center, through the hole in the center of the motor. From the bottom side, screw on the magnet-nut assembly and tighten the nut onto the screw with an hex key and wrench.
## 2.4. Mounting the Encoder
1. Print one of the encoder mounts included under "CAD Files" in this repo
2. Embed Thin Profile M2.5 hex nuts into the provided locations in the encoder mount.
3. Attach a ribbon of 6 jumper cables to the following pins of the encoder header in the following order for future simplicity:
   1. 3V3
   2. CLK (SCK)
   3. MISO
   4. MOSI
   5. CSn
   6. GND
4. Pass the jumper wires through the wire pass-thru if using the encoder mount for aluminum extrusions.
5. Attach the encoder to the mount with 4x M2.5 x 8mm socket head screws.
## 2.5. Putting it All Together
1. Attach the motor to the mount with M4 x 10mm or M4 x 14mmscrews
2. Attach the motor mount to an aluminum rail or install standoffs through the four outer mounting points as desired

# 3. Setting up the ODrive Board and connecting motors and encoders
### 3.0.1. Powering the Odrive
1. Power Cable Assembly (case dependent)
   1. The ODrive 24V v3.6 needs a 12-24V power supply to the "DC +/-" terminals
   2. I built one with 10AWG wire from the board to an XT60 connector, and then a longer cable from the XT60 connector to the power supply with banana plugs on the end.
2. Solder block connectors to the M0 and M1 connection terminals
### 3.0.2. Wiring the Encoder and Motor
1. Connect the motor power wires to the block terminals (for the U8II, the motor wires are in the order they need to be connected to the ODrive)
2. Connect the Encoder wires as follows (AS5047P -> ODrive):
   1. 3.3V -> 3.3V
   2. CSn -> GPIO7 (can be connected to any GPIO pin #3-8, just change motor setup accordingly) 
   3. CLK -> SCK
   4. MISO -> MISO
   5. MOSI -> MOSI
   6. GND -> GND
   
# 4. Anaconda/Odrivetool Setup and Config
*NOTE: For more additional setup information, consult the **[ODrive Documentation](https://docs.odriverobotics.com/)***
*<br>The following is copied from the ODrive Documentation. I recommend reading the Getting Started Guide before continuing*
## 4.1. Downloading and Installing Tools
Most instructions in this guide refer to a utility called `odrivetool`, so you should install that first.
### 4.1.1. Windows
1. Install Python 3. We recommend the Anaconda distribution because it packs a lot of useful scientific tools, however you can also install the standalone python.
  * __Anaconda__: Download the installer from [here](https://www.anaconda.com/download/#windows). Execute the downloaded file and follow the instructions.
  * __Standalone Python__: Download the installer from [here](https://www.python.org/downloads/). Execute the downloaded file and follow the instructions.
  * If you have Python 2 installed alongside Python 3, replace `pip` by `C:\Users\YOUR_USERNAME\AppData\Local\Programs\Python\Python36-32\Scripts\pip`. If you have trouble with this step then refer to [this walkthrough](https://www.youtube.com/watch?v=jnpC_Ib_lbc).
2. Launch the command prompt.
  * __Anaconda__: In the start menu, type `Anaconda Prompt` <kbd>Enter</kbd>
  * __Standalone Python__: In the start menu, type `cmd` <kbd>Enter</kbd>
3. Install the ODrive tools by typing `pip install --upgrade odrive` <kbd>Enter</kbd>


### 4.1.2. OSX
We are going to run the following commands for installation in Terminal.
1. If you don't already have it, install homebrew:
```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
2. Install python:
```bash
brew install python
```
3. If you get the error: `Error: python 2.7.14_2 is already installed`, then upgrade to Python 3 by running:
```bash
brew upgrade python
```
4. The odrive tools uses libusb to communicate to the ODrive:
```bash
brew install libusb
```
5. Now that you have Python 3 and all the package managers, run:
```bash
pip3 install --upgrade odrive
```
## 4.2. Start `odrivetool`
To launch the main interactive ODrive tool, type `odrivetool` <kbd>Enter</kbd>. Connect your ODrive and wait for the tool to find it. If it doesn't connect after a few seconds refer to the [troubleshooting page](troubleshooting.md#usb-connectivity-issues). Now you can, for instance type `odrv0.vbus_voltage` <kbd>Enter</kbd> to inpect the boards main supply voltage.
It should look something like this:

```text
ODrive control utility v0.5.1
Please connect your ODrive.
Type help() for help.

Connected to ODrive 306A396A3235 as odrv0
In [1]: odrv0.vbus_voltage
Out[1]: 11.97055721282959
```
# 5. ODrive Calibration
*Note: The following is a sample calibration for moderately low power application*
The following commands will configure the U8II and AS5047P to 
## 5.1. Motor Configuration
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
odrv0.save_configuration()
odrv0.config.brake_resistance = 0.5
```
## 5.2. Encoder Calibration
The GPIO pin configuration has to correspond to the GPIO pin the encoder is connected to. GPIO pins 1 and 2 are reserved for UART communication from the Teensy Board.
```
odrv0.axis0.encoder.config.abs_spi_cs_gpio_pin = 7  # or which ever GPIO pin you choose
odrv0.axis0.encoder.config.mode = ENCODER_MODE_SPI_ABS_AMS
odrv0.axis0.encoder.config.cpr = 2**14
odrv0.save_configuration()
odrv0.reboot()
```

## 5.3. Motor Calibration and Startup
```
odrv0.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
odrv0.axis0.requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION
odrv0.save_configuration()
odrv0.reboot()

odrv0.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL

odrv0.axis0.controller.input_pos = 1 # example of positional control
```

## 5.4. Motor Tuning
(Ask John) 

# 6. Controlling the ODrive with a Teensy Microcontroller (or Equivalent)
Attach a 3 wire jumper ribbon from the microcontroller to the ODrive with the following connections:

| Teensy | ODrive     |
|--------|------------|
| GND    | GND        |
| Rx (0) | Tx (GPIO1) | 
| Tx (1) | Rx (GPIO2) | 

## Downloading PlatformIO in VSCode to Run Scripts
This is my recommended method, but if you have a preferred way of interfacing with Arduino-based MCs, then feel free to use any other method.
1. Download the PlatformIO extension in VSCode
   1. Follow any related installation instructions
2. Click on the option "Clone Git Project" from the PlatformIO menu
3. Configure the Port the microcontroller is connected to in the file `platformio.ini`
   1. Change the value of `monitor_port` to suit your specific use case.
