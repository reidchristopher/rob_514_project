#ifndef MOVING_AVERAGE_FILTER_H
#define MOVING_AVERAGE_FILTER_H

class MovingAverageFilter
{
private:
	float* values;
	float current_average;
	int filter_size;
	int current_index;
	
	float getOldestValue();

	void setNewValue(float value);

public:
	MovingAverageFilter(int filter_size);
	
	~MovingAverageFilter();
	
	float getValue(float value);
};

#endif