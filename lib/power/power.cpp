#include "power.hpp"

extern "C"
{
#include "mainsFilter.h"
#include "lowpassFilter.h"
#include "lagFilter.h"
}

mainsFilter current_filter;
mainsFilter voltage_filter;
lagFilter current_lag_filter;
lagFilter voltage_lag_filter;
lowpassFilter lowpass_filter;

void filters_init()
{
	mainsFilter_init(&current_filter);
	mainsFilter_init(&voltage_filter);
	lagFilter_init(&current_lag_filter);
	lagFilter_init(&voltage_lag_filter);
	lowpassFilter_init(&lowpass_filter);
	filter_init_done = true;
}

bool IRAM_ATTR get_power(Power *power)
{
	try
	{
		int32_t _current = 0;
		int32_t _voltage = 0;
		int32_t _power = 0;

		filter_mains(&_voltage, &_current);
		_power = _current * _voltage;
		filter_power(&_power);

		power->current = _current;
		power->voltage = _voltage;
		power->power = _power;
		synchronize(power);
		return true;
	}
	catch (...)
	{
		Serial.println("Error in get_power");
		return false;
	}
}

/**
 * @brief Applies a 40 Hz to 60 Hz bandpass filter to the mains current and voltage.
 * 
 * @param voltage pointer to the voltage variable
 * @param current pointer to the current variable
 */
void IRAM_ATTR filter_mains(int32_t *voltage, int32_t *current)
{
	mainsFilter_put(&current_filter, *current);
	mainsFilter_put(&voltage_filter, *voltage);
	*current = mainsFilter_get(&current_filter) - 1;
	*voltage = mainsFilter_get(&voltage_filter);
	*current = *current >= 2 || *current <= -1 ? *current : 0;
}

/**
 * @brief Applies a lowpass filter with 2 Hz cutoff to the power.
 * 
 * @param power pointer to the power variable
 */
void IRAM_ATTR filter_power(int32_t *power)
{
	lowpassFilter_put(&lowpass_filter, *power);
	*power = lowpassFilter_get(&lowpass_filter);
	*power = *power >= 0 ? *power : 0;
}

/**
 * @brief After filtering the power, this function synchronizes the current and voltage values
 * with the power value by adding the same delay to them as power. 
 * 
 * @param power pointer to the power struct
 */
void IRAM_ATTR synchronize(Power *power)
{
	lagFilter_put(&current_lag_filter, power->current);
	lagFilter_put(&voltage_lag_filter, power->voltage);
	power->current = lagFilter_get(&current_lag_filter);
	power->voltage = lagFilter_get(&voltage_lag_filter);
}

void IRAM_ATTR print_power(Power *filtered_power)
{
	Serial.print("C:");
	Serial.print(filtered_power->current);
	Serial.print(", V:");
	Serial.print(filtered_power->voltage);
	Serial.print(", P:");
	Serial.print(filtered_power->power);
	Serial.println();
}