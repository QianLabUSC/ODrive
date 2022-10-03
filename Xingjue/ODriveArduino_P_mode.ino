// includes
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ODriveArduino.h>
// Printing with stream operator helper functions
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }


////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

// Below are some sample configurations.
// You can comment out the default Teensy one and uncomment the one you wish to use.
// You can of course use something different if you like
// Don't forget to also connect ODrive GND to Arduino GND.

// Teensy 3 and 4 (all versions) - Serial1
// pin 0: RX - connect to ODrive TX
// pin 1: TX - connect to ODrive RX
// See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
HardwareSerial& odrive_right_serial = Serial1;
HardwareSerial& odrive_left_serial = Serial2;

// Arduino Mega or Due - Serial1
// pin 19: RX - connect to ODrive TX
// pin 18: TX - connect to ODrive RX
// See https://www.arduino.cc/reference/en/language/functions/communication/serial/ for other options
// HardwareSerial& odrive_serial = Serial1;

// Arduino without spare serial ports (such as Arduino UNO) have to use software serial.
// Note that this is implemented poorly and can lead to wrong data sent or read.
// pin 8: RX - connect to ODrive TX
// pin 9: TX - connect to ODrive RX
// SoftwareSerial odrive_serial(8, 9);


// ODrive object
ODriveArduino odrive_right(odrive_right_serial);
ODriveArduino odrive_left(odrive_left_serial);

String leg_name[4]={"RF","LF","LR","RR"};

int leg_state[4]={0,0,0,0}; //0= air, 1=contact;
long leg_peak_start_time[4]={0,0,0,0}; //to record when a peak starts
long leg_peak_end_time[4]={0,0,0,0}; //to record when a peak starts
float max_peak_current[4]={0,0,0,0};
float last_peak_current[4]={0,0,0,0};
 
float gait[]={0,0.5,0.5}; // (\phi_1, \phi_2, \phi_3) = [LF-RF,LR-RF,RR-RF];

void adjust_gait(int leg, float I, float last_I){
  if (I>last_I*1.2){
      gait[leg-1]=gait[leg-1]+0.1;
  }
  

  gait[leg-1]=fmod(gait[leg-1],1.0);
  
}
void peak_detection(int leg, long t){
  int axis;
  float peak_threshold=4; //in A
  float velocity_threshold=0.25; //in turn/s

  if (leg>=4) return;
  
  if (leg==0 || leg==1){
    axis=0;
  }
  else{
    axis=1;
  }
  float current;
  if (leg==0 || leg==3){ //on the right side, the current is negative while the velocity is positive
      odrive_right_serial << "r axis" << axis << ".motor.current_control.Iq_measured\n";
      current =  odrive_right.readFloat();
      if (current<-peak_threshold && leg_state[leg]==0 && t-leg_peak_end_time[leg]>500) {
        //Serial << "Leg " << leg_name[leg] << " Peak Detected with Current: " << current << '\n';
        odrive_right_serial << "r axis" << axis << ".encoder.vel_estimate\n";
        float velocity= odrive_right.readFloat();
        if (velocity<velocity_threshold){
            leg_state[leg]=1;
            leg_peak_start_time[leg]=t;
        }
      }
      if (leg_state[leg]==1 && current<max_peak_current[leg]){
        max_peak_current[leg]=current;
      }
      if (current>-peak_threshold && leg_state[leg]==1 && t-leg_peak_start_time[leg]>500) {
        Serial << "Leg " << leg_name[leg] << " Peak Detected with Current: " << max_peak_current[leg] << '\n';
        leg_state[leg]=0;
        leg_peak_end_time[leg]=t;
        adjust_gait(leg,-max_peak_current[leg],-last_peak_current[leg]);
        
        last_peak_current[leg]=max_peak_current[leg];
        max_peak_current[leg]=0;
      }
  }      
  else{ //on the left side, the current is positive while the velocity is negative
      odrive_left_serial << "r axis" << axis << ".motor.current_control.Iq_measured\n";
      current =  odrive_left.readFloat();
     
      /*if (leg==1 && current>100) {
              odrive_left_serial << "r axis" << axis << ".motor.current_control.Iq_measured\n";
              current =  odrive_left.readFloat();
      }*/
      if (current>peak_threshold && leg_state[leg]==0 && t-leg_peak_end_time[leg]>500) {
        //Serial << "Leg " << leg << " Peak Detected with Current: " << current << '\n';
        odrive_left_serial << "r axis" << axis << ".encoder.vel_estimate\n";
        float velocity= odrive_left.readFloat();
        if (velocity>-velocity_threshold){
          leg_state[leg]=1;
          leg_peak_start_time[leg]=t;
        }
      }
      if (leg_state[leg]==1 && current>max_peak_current[leg]){
        max_peak_current[leg]=current;
      }
      if (current<peak_threshold && leg_state[leg]==1 && t-leg_peak_start_time[leg]>500) {
        Serial << "Leg " << leg_name[leg] << " Peak Detected with Current: " << max_peak_current[leg] << '\n';
        leg_state[leg]=0;
        leg_peak_end_time[leg]=t;
        adjust_gait(leg,max_peak_current[leg],last_peak_current[leg]);
                
        last_peak_current[leg]=max_peak_current[leg];
        max_peak_current[leg]=0;
      }
  }
  
  
}



void setup() {
  // ODrive uses 115200 baud
  odrive_right_serial.begin(115200);
  odrive_left_serial.begin(115200);

  // Serial to PC
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open

  Serial.println("ODriveArduino");
  Serial.println("Setting parameters...");

  // In this example we set the same parameters to both motors.
  // You can of course set them different if you want.
  // See the documentation or play around in odrivetool to see the available parameters
  for (int axis = 0; axis < 2; ++axis) {
    odrive_right_serial << "w axis" << axis << ".controller.config.vel_limit " << 10.0f << '\n';
    odrive_right_serial << "w axis" << axis << ".motor.config.current_lim " << 15.0f << '\n';
    // This ends up writing something like "w axis0.motor.config.current_lim 10.0\n"
    odrive_left_serial << "w axis" << axis << ".controller.config.vel_limit " << 10.0f << '\n';
    odrive_left_serial << "w axis" << axis << ".motor.config.current_lim " << 15.0f << '\n';
    
  }

  Serial.println("Ready!");
  Serial.println("Send the character '0' or '1' to calibrate respective motor (you must do this before you can command movement)");
  Serial.println("Send the character 's' to exectue test move");
  Serial.println("Send the character 'c' to exectue continuous move");  
  Serial.println("Send the character 'i' to idel motors");    
  Serial.println("Send the character 't' to test the current of RF");      
  Serial.println("Send the character 'b' to read bus voltage (right side)");
  Serial.println("Send the character 'p' to read motor positions in a 10s loop (right side)");
}

void loop() {

  if (Serial.available()) {
    char c = Serial.read();

    // Run calibration sequence
    if (c == '0' || c == '1') {
      int motornum = c-'0';
      int requested_state;

      requested_state = AXIS_STATE_MOTOR_CALIBRATION;
      Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
      if(!odrive_right.run_state(motornum, requested_state, true)) return;
      if(!odrive_left.run_state(motornum, requested_state, true)) return;

      requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
      if(!odrive_right.run_state(motornum, requested_state, true, 25.0f)) return;
      if(!odrive_left.run_state(motornum, requested_state, true, 25.0f)) return;
      
      requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL;
      Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
      if(!odrive_right.run_state(motornum, requested_state, false /*don't wait*/)) return;
      if(!odrive_left.run_state(motornum, requested_state, false /*don't wait*/)) return;
      
    }

    if (c == 'i') {
      int requested_state;

      requested_state = AXIS_STATE_IDLE;
      Serial << "All Motors" << ": Requesting state " << requested_state << '\n';
      if(!odrive_right.run_state(0, requested_state, true)) return;
      if(!odrive_left.run_state(0, requested_state, true)) return;  
      if(!odrive_right.run_state(1, requested_state, true)) return;
      if(!odrive_left.run_state(1, requested_state, true)) return;  
       
    }
    
    if (c == 't') {
      odrive_right_serial << "r axis" << 0 << ".motor.current_control.Iq_measured\n";
      float current_rh =  odrive_right.readFloat();         
    }
    
    
    // 1 cycle test move
    if (c == 's') {
      Serial.println("Executing test move");
      for (float ph = 0.0f; ph < 1.00f; ph += 0.0025f) {
        float pos_m0 = ph;
        float pos_m1 = ph;
        odrive_left.SetPosition(0, 1.05-pos_m0);
        odrive_left.SetPosition(1, 1.05-pos_m1);
        
        odrive_right.SetPosition(0, pos_m0);
        odrive_right.SetPosition(1, pos_m1);

        odrive_left_serial << "r axis" << 0 << ".motor.current_control.Iq_measured\n";
        //odrive_left_serial << "r axis" << 0 << ".encoder.vel_estimate\n";
       
        float current_lf =  odrive_left.readFloat();
        
        Serial.println(current_lf);
      
        delay(5);
      }
    }
    
    if (c == 'c') {
      Serial.println("Executing test move, press any letter to exit");
      long start=millis();
      long last_time=millis();
      int rpm=40;
      int time_interval=5; //in ms
      float pos_interval=(rpm/60.0)/(1000/time_interval);
      float pos_m0 = 0;
      float pos_m1 = 0;

      long leg_counter=0;
      while(true){

        leg_counter++;
        long current_time =millis();
            
        if (current_time-last_time >time_interval){
            odrive_left.SetPosition(0, 1.05-(pos_m0+gait[0]));
            odrive_left.SetPosition(1, 1.05-(pos_m1+gait[1]));
          
            odrive_right.SetPosition(0, pos_m0);
            odrive_right.SetPosition(1, pos_m1+gait[2]);

            pos_m0 = fmod(pos_m0+pos_interval,1.0);
            pos_m1 = fmod(pos_m1+pos_interval,1.0);
            last_time=current_time;
            int leg_testing=(leg_counter)%4;

            peak_detection(leg_testing,current_time);
            while (Serial1.available()){
                  char c = Serial1.read();
            }
            
            while (Serial2.available()){
                  char c = Serial2.read();
            }
            
        }
        
          
        
        if (Serial.available()){ 
          char temp=Serial.read();
          if (temp>=64) {
            Serial.println("Motion stopped");
            break;
          }
        }  
      }
    }

    // Read bus voltage
    if (c == 'b') {
      odrive_right_serial << "r vbus_voltage\n";
      Serial << "Vbus voltage: " << odrive_right.readFloat() << '\n';
    }

    // print motor positions in a 10s loop
    if (c == 'p') {
      static const unsigned long duration = 10000;
      unsigned long start = millis();
      while(millis() - start < duration) {
        for (int motor = 0; motor < 2; ++motor) {
          Serial << odrive_right.GetPosition(motor) << '\t';
        }
        Serial << '\n';
      }
    }
  }
}
