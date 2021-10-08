#include <string>

using std::string;

#pragma once
#ifndef __CONSOLE__
#define __CONSOLE__

namespace Console {

// Persists the user's command.
extern string cmd_str;

enum Command {
    NONE,
    UNKNOWN,
    QUIT,
    GAIT,
};

Console::Command listen();

Console::Command interpret();

}  // namespace Console

#endif