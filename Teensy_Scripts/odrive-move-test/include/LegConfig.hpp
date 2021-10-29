#pragma once

#include <HardwareSerial.h>

#include <utility>
#include <vector>

#include "UtilityHelpers.hpp"
#include "../lib/ODriveArduino/ODriveArduino.h"

class LegConfig {
    public:
        // Constants
        static const int BAUD = 115200;
        static const int MOTOR_COUNT = 2;
        
        /** 
         * @param serial_ptr : ODrive Board Serial Connection
         * @param axis0_init_offset: angular offset 0
         * @param axis1_init_offset: angular offset 1
         */
        LegConfig(
            HardwareSerial *serial_ptr,
            float axis0_init_offset, 
            float axis1_init_offset
        );
        
        //Access Methods
        
        const float init_offset(int axis) const;
        std::pair<ODriveArduino, HardwareSerial *> &odrv() { return _odrv; };
        

        // ODrive Serial Setup
        void serialSetup();

        void legSetup(int calibration_mode = 3);

        // Full Leg Setup
        void setup();

        /**
         * @brief checks the ODrive Axis for errors
         * @return 	TRUE: there is an error in @param axis
         * @return  FALSE: there is no error in @param axis
         */ 
        bool ErrorCheck(int axis);

        void EStop();
        
  
        // *Data Members:
    private:
        // ODrive Board
        std::pair<ODriveArduino, HardwareSerial *> _odrv;

        /**
         * Initial Angles for motors 0 and 1 for initial leg state upon startup
         * @units: rotations
         * @bounds: [0,1)
         */
        const float _axis0_init_offset;
        const float _axis1_init_offset; 

};

