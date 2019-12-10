#include "Arduino.h"
#include "SetPointPot.h"

PotSetPoint::PotSetPoint(int analog_input_pin, float min_val, float max_val)
{
	min = min_val;
	max = max_val;
	pin = analog_input_pin;
}

float PotSetPoint::getSetPoint()
{
	return (float(analogRead(pin)) / 1023) * (max - min) + min;
}