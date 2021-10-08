#include "console.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include <sstream>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "UtilityHelpers.hpp"

string Console::cmd_str;

using std::istringstream;

Console::Command Console::listen() {
    if (Serial.available() == false) {
        return NONE;
    }
    char c = Serial.read();
    Serial << c;

    // Note: "Enter" is considered to be a Carriage Return.
    if (c == '\n') {
        // do nothing.
    } else if (c == '\r') {
        Serial.println("");
        return interpret();
    } else {
        cmd_str.push_back(c);
        return NONE;
    }
}

Console::Command Console::interpret() {
    string cmd = cmd_str;

    // Reset string.
    cmd_str = "";

    if (cmd == "q") {
        return QUIT;
    }
    if (cmd == "quit") {
        return QUIT;
    }
    Serial.println(cmd.c_str());
    if (cmd.substr(0, 4) == "gait") {
        istringstream iss;
        iss.str(cmd);
        string _;
        iss >> _;  // discard

        int L_FORE, R_HIND, L_HIND;
        iss >> L_FORE;
        iss >> R_HIND;
        iss >> L_HIND;
        Serial << L_FORE << R_HIND << L_HIND;

        if (iss.fail()) {
            Serial.println("Could not parse gait.");
        }

        return GAIT;
    }
    return UNKNOWN;
}