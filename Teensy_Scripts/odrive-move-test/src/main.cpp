#include "PositionControl.hpp"
#include "SinusoidalGait.hpp"
#include "LegConfig.hpp"
#include "UtilityHelpers.hpp"
#include "TorqueHelpers.hpp"



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
		case 'r' :
			{
				float time;
				float ms = millis();
				float start = ms / 1000.0;

				// ? start at neutral leg position?
				float theta;
				float gamma;
				
				Serial.println("Enter Mode #: ");
				while (Serial.available() == false) {}
				int mode = Serial.parseInt();

				while (Serial.read() != 'q') {
					time = ms - start;
					RadialLegMovement(Leg, time, radial_gait_params[mode], theta, gamma);
					delay(1);
					ms = millis() / 1000.0;
				}
			}
			break;
		case 's' :
			{
				float time;
				float ms = millis();
				float start = ms / 1000.0;

				// ? start at neutral leg position?
				float theta;
				float gamma;
				
				Serial.println("Enter Gait #: ");
				while (Serial.available() == false) {}
				int mode = Serial.parseInt();

				while (Serial.read() != 'q') {
					time = ms - start;
					GaitMovement(Leg, time, Gaits[mode], theta, gamma);
					delay(1);
					ms = millis() / 1000.0;
				}
			}	
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
