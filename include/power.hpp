#ifndef POWER_HPP
#define POWER_HPP

#include <Arduino.h>
#include "board.hpp"

extern "C"
{
#include "filters/mainsFilter.h"
#include "filters/lowpassFilter.h"
#include "filters/lagFilter.h"
}

/**
 * @brief struct to hold the power data for the current sample.
 */
struct Power
{
	int32_t power = 0;
	int32_t voltage = 0;
	int32_t current = 0;
};

// filter initialization
mainsFilter current_filter;
mainsFilter voltage_filter;
lagFilter current_lag_filter;
lagFilter voltage_lag_filter;
lowpassFilter lowpass_filter;
static bool filter_init = false;

/**
 * @brief Initializes the power measurement filters. 
 * This must be called before using get_power().
 */
void filters_init()
{
	mainsFilter_init(&current_filter);
	mainsFilter_init(&voltage_filter);
	lagFilter_init(&current_lag_filter);
	lagFilter_init(&voltage_lag_filter);
	lowpassFilter_init(&lowpass_filter);
	filter_init = true;
}

/**
 * @brief This function is called at a frequency of 200Hz. The mains frequency is expected
 * to be around 60 Hz. So 40 - 70 Hz bandpass filters are applied to the voltage and current
 * readings. Instantaneous power is calculated by multiplying the voltage and current.
 * The power is then passed through a low pass filter to get a valid RMS value.
 * 
 * @param filtered_power pointer to variable containing filtered power
 * @return true if calculation was successful
 * @return false if calculation was unsuccessful
 */
bool IRAM_ATTR get_power(Power *filtered_power)
{
	if (!filter_init)
	{
		filters_init();
	}
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

/**
 * @brief prints the power data to the serial port in C, V, P format.
 * 
 * @param filtered_power pointer to variable containing filtered power
 */
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

#endif