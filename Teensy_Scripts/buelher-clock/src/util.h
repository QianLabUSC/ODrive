#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <Arduino.h>

// Printing with stream operator helper functions
template <class T>
inline Print &operator<<(Print &obj, T arg)
{
    obj.print(arg);
    return obj;
}
template <>
inline Print &operator<<(Print &obj, float arg)
{
    obj.print(arg, 4);
    return obj;
}


void formatTime(char *out)
{
	int minutes = minute();
	int seconds = second();
	unsigned long time = millis();
	int ms = int(time % 1000);

	char buffer[12];

	sprintf(buffer, "%d:%d:%d", minutes, seconds, ms);

	for (int i = 0; i < 10; i++)
	{
		out[i] = buffer[i];
	}
}