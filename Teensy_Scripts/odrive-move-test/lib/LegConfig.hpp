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
        LegConfig(
            HardwareSerial *serial_ptr,
            float axis0_init_offset, 
            float axis1_init_offset
        );
        
        //Access Methods
        
        const float init_offset(int axis) const;
        const std::pair<ODriveArduino, HardwareSerial *> &odrv() const { return _odrv; };
        
        // ODrive Serial Setup
        void serialSetup() const;

        void legSetup(int calibration_mode = 3) const;

        // Full Leg Setup
        void setup() const;

        /**
         * @brief checks the ODrive Axis for errors
         * @return 	TRUE: there is an error in @param axis
         * @return  FALSE: there is no error in @param axis
         */ 
        bool ErrorCheck(int axis) const;
        
  


        // *Data Members:
    private:
        // ODrive Board
        std::pair<ODriveArduino, HardwareSerial *> _odrv;

        /**
         * Initial Angles for motors 0 and 1 for initial leg state upon startup
         * @units: rotations
         * @bounds: [0,1)
         */
        float _axis0_init_offset;
        float _axis1_init_offset; 

};

