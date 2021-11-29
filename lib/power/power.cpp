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
}

bool IRAM_ATTR get_power(Power *filtered_power)
{
	try
	{
		int32_t current = 0;
		int32_t voltage = 0;
		int32_t power = 0;

		mainsFilter_put(&current_filter, analogRead(CURRENT_SENSOR_PIN));
		mainsFilter_put(&voltage_filter, analogRead(VOLTAGE_SENSOR_PIN));

		current = mainsFilter_get(&current_filter) - 1;
		voltage = mainsFilter_get(&voltage_filter);

		current = current >= 2 || current <= -1 ? current : 0;
		power = current * voltage;
		power = power >= 0 ? power : 0;

		lagFilter_put(&current_lag_filter, current);
		lagFilter_put(&voltage_lag_filter, voltage);
		lowpassFilter_put(&lowpass_filter, power);

		current = lagFilter_get(&current_lag_filter);
		voltage = lagFilter_get(&voltage_lag_filter);
		power = lowpassFilter_get(&lowpass_filter);

		filtered_power->current = current;
		filtered_power->voltage = voltage;
		filtered_power->power = power;

		return true;
	}
	catch (...)
	{
		Serial.println("Error in get_power");
		return false;
	}
}

void print_power(Power *filtered_power)
{
	Serial.print("C:");
	Serial.print(filtered_power->current);
	Serial.print(", V:");
	Serial.print(filtered_power->voltage);
	Serial.print(", P:");
	Serial.print(filtered_power->power);
	Serial.println();
}
