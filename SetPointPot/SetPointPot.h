#ifndef POT_SET_POINT_H
#define POT_SET_POINT_H

class PotSetPoint
{	
private:
	float min, max;
	int pin;
	
	
public:
	PotSetPoint(int analog_input_pin, float min_val, float max_val);
	
	float getSetPoint();
	
};

#endif