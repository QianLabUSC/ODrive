# Physical Design
Weight: ~2.5kg, including all legs and battery
Dimensions: ???

### Torque
We tested a single leg by
1. entering closed loop control mode
2. Commanding it to hold its position
3. hanging a weight on the leg
4. Seeing if the leg could support the weight
	- we also noted the current drawn by the motor

We observed the following currents
Steady State Weight Test
- 200g: 0.5A
- 300g: 1.0A
- 400g: 1.7A
- 500g: 
	- leg sags, current maxes out 1.7A
	- increased current limit to 15A, 
		- now draws 3.2A
	- motor noticably warmer


# Electronics
Components
- 4x Antigravity Motors, [380kv](https://store.tmotor.com/goods.php?id=440)
- 4x [CUI AMT 102 Encoders](https://odriverobotics.com/shop/cui-amt-102)
- 2x ODrive  3.6 24V boards
- 1x Teensy 3.5 Arduino
	- [UART Pins](https://www.pjrc.com/teensy/td_uart.html)

Leg Control
- each leg consists one motor and one encoder connected to an ODrive control board
	- the encoder informs the board of the motor’s rotation

### Wiring
Each leg has the following wires
- 3 wires from the motor to the ODrive board. This provides the 3 phase connection to drive the motor.
	- connected via a yellow plug
- 5 wires from the CUI Encoder to the ODrive board
	- the wires are bundled together, and have been reduced in length via re-soldering them
- The axis for motor wires and encoder wires must match
	- i.e. the Axis 0 motor wires must go to the same leg as the Axis 0 encoder wires

Each ODrive board has the following wires
- ground wire to the Teensy
- Rx and Tx wires forming a UART connection with the Teensy
- power lines to a power source
# Code
### Overview & Setup
“A -> B" means "A controls B"
Teensy -> ODrive Boards -> Motors

The teensy runs a C++ program
- the program is located in the RoboLand ODrive Repository, currently under “Buelher Clock” (21.12.03) 

The program is loaded from some computer over a micro USB cable, using the [PlatformIO](https://platformio.org/install/ide?install=vscode) software for Visual Studio Code.

### Zeroing
To fix each motor’s “zero position”, perform the following
- power the motor, leave it in idle mode
- connect the ODrive board to the `odrivetool` command line
- physically move the leg to the desired zero position
- in the `odrivetool` command line, run `odrv0.axis0.encoder.set_linear_count(0)`
	- `axis0` and `odrv0` may change based on the board and motor
- save the board configuration

### Structure
Classes: 
- Each leg is represented by a `LegConfig` object.
- The robot is represented with a `RoboConfig` object, which contains 4 `LegConfig` objects.
- The program runs using a `Gait` object.
	- for example, using `Gait::TROTTING` will cause the robot to adopt a trotting `Gait`.

### Dependencies
Relies in `HardwareSerial.h`, `SoftwareSerial.h`
==TENTATIVE== these are built into PlatformIO
The ODriveArduino dependency is included in the repository.