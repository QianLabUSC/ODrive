#include "mode_facilitator.hpp"



// *GLOBAL VARIABLES
// NUMBER OF MOTORS CONNECTED TO ODRIVE
int NUM_MOTORS = 1;
int WORKSPACE = 0.75f;

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
		case 's' :
			SinusoidalGait(Leg);	
			break;
		case 'e' :
			Leg.odrv().first.SetPosition(0, 0.0);
			Leg.odrv().first.SetPosition(1, 0.0);
			break;
		case 'w':
			*(Leg.odrv().second) << "sr" << "\n";
			break;
		case 'q' :
			Leg.EStop();
			break;
	}

	
}
