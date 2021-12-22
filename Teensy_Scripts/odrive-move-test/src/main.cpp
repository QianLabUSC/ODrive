#include "ModeFacilitator.hpp"

// *GLOBAL VARIABLES
// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;
int WORKSPACE = 0.75f;

/**
 * The following two lines of code are essential for setting up the ODrive
 * object:
 */
LegConfig Leg = LegConfig(&Serial1, 0.0f, 0.0f);

void setup() { Leg.setup(6); } //runs encoder offset calibration in setup


// MAIN CONTROL LOOP
void loop() {
	while (Serial.available() == false) return;


	char c = Serial.read();

	switch(c) {
		// Performs Radial Leg Movement
		case 'r' :
			RadialGait(Leg);
			break;
		// Performs Sinusoidal Leg Movement
		// !CURRENTLY BEING DEBUGGED
		case 's' :
			SinusoidalGait(Leg);	
			break;
		// RESETS LEG POSITION
		case 'e' :
			Leg.odrv().first.SetPosition(0, 0.0);
			Leg.odrv().first.SetPosition(1, 0.0);
			break;
		// Resets the ODrive board
		case 'w':
			*(Leg.odrv().second) << "sr" << "\n";
			break;
		// Triggers the ESTOP
		case 'q' :
			Leg.EStop();
			break;
	}

	
}
