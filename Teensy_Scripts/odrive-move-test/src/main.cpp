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

	switch(c) {
		case 's':
			float t = millis();
			float start = t;

			// ? start at neutral leg position?
			float theta;
			float gamma;

			while (Serial.read() != 'q') {
				RadialLegMovement(Leg, t - start, radial_gate_params[2], theta, gamma); 
			}
			break;
	}

	

}
