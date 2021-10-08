#include "console.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include "../lib/ODriveArduino/ODriveArduino.h"
#include "UtilityHelpers.hpp"

string Console::cmd_str;

Console::Command Console::listen() {
    if (Serial.available() == false) {
        return NONE;
    }
    char c = Serial.read();
    Serial << c;

    // Note: "Enter" is considered to be a Carriage Return.
    if (c == '\r') {
        Serial.println("");
        Serial.print("> ");

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
    } else if (cmd == "quit") {
        return QUIT;
    } else {
        return UNKNOWN;
    }
}