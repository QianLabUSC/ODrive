#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "../lib/ODriveArduino/ODriveArduino.h"
#include <cstdlib>
#include <cstdio>
#include <string>

#include "TorqueHelpers.hpp"
#include "UtilityHelpers.hpp"
#include "buelher.hpp"
#include "ODriveState.hpp"
#include "run_clock.hpp"

////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

HardwareSerial &odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;

void setup()
{
    // ODrive uses 115200 baud
    odrive_serial.begin(115200);

    // Serial to PC
    Serial.begin(115200);
    while (!Serial)
        ; // wait for Arduino Serial Monitor to open

    Serial.println("ODriveArduino");
    Serial.println("Setting parameters...");
    Serial.println(odrive.getBoardInfo()); //prints the firmware version of the ODrive (confirms connection)

    for (int axis = 0; axis < NUM_MOTORS; ++axis)
    {
        /**
		 * Startup Calibration for ODrive
		 */
        int requested_state;
        requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
        Serial << "Axis" << axis << ": Requesting state " << requested_state << '\n';
        odrive.run_state(axis, requested_state, true);

        // Changes motor controller input mode to input PASSTHROUGH mode
        odrive_serial << "w axis" << axis << ".controller.input_mode " << 3 << "\n";

        if (checkError(0, odrive, odrive_serial))
        {
            Serial.println("Error in Motor Axis 0");
        }
    }

    // serial monitor interface
    Serial.println("Motor Armed & Ready");
    Serial.println("Command Menu:");
    Serial.println("	'0' or '1' -> calibrate respective motor (you must do this before you can command movement)");
    Serial.println("	'l' -> enter closed loop control.");
    Serial.println("	'b' -> reads bus voltage");
    Serial.println("	'q' -> Sends motors to IDLE STATE");
    Serial.println("	'c' -> Execute Buelher Clock");
}

// MAIN CONTROL LOOP
void loop()
{
    if (Serial.available() == false)
        return;
    
    char c = Serial.read();

    /**
     * @input: 0 or 1
     * @brief: Run calibration sequence
     */
    if (c == '0' || c == '1')
    {
        int motornum = c - '0';
        int requested_state;

        requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
        Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
        if (!odrive.run_state(motornum, requested_state, true))
            return;
    }

    /**
     * @input: 'l'
     * @brief: starts closed loop control
     */
    if (c == 'l')
        loop_control(c, odrive);

    // Read bus voltage
    if (c == 'b')
    {
        odrive_serial << "r vbus_voltage\n";
        Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
    }

    /**
     * @input: 'q'
     * @brief: sets motor state to IDLE
     */
    if (c == 'q')
        idle_state(c, odrive);

    /**
     * @input: 'b'
     * !Note: In Development
     * @brief: runs a Buelher Clock
     */
    if (c == 'c')
    {
        run_clock(c, odrive);
    }
}
