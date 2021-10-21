#include "PositionControl.hpp"
#include "LegConfig.hpp"
#include "UtilityHelpers.hpp"
#include "TorqueHelpers.hpp"



// *GLOBAL VARIABLES
// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;
int WORKSPACE = 0.75f;

LegConfig Leg = LegConfig((HardwareSerial*) &Serial, 0.0f, 0.0f);

void setup() { Leg.setup(); }


// MAIN CONTROL LOOP
void loop() {
	while (Serial.available() == false) return;


	char c = Serial.read();
	

}
