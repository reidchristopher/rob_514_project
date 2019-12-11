#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(int filter_size)
{
	current_index = 0;
	this->filter_size = filter_size;
	values = new float[filter_size];
	current_average = 0.0f;
	
	for (int i = 0; i < filter_size; i++)
	{
		values[i] = 0.0f;
	}
}

MovingAverageFilter::~MovingAverageFilter()
{
	delete[] values;
}

float MovingAverageFilter::getOldestValue()
{
	return values[current_index];
}

void MovingAverageFilter::setNewValue(float value)
{
	values[current_index] = value;
	
	current_index = (current_index + 1) % filter_size;
}

float MovingAverageFilter::getValue(float value)
{
	float removed_value = getOldestValue();
	setNewValue(value);

	current_average -= removed_value / filter_size;
	current_average += value / filter_size;
	
	return current_average;
}