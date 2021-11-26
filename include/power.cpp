#include "power.hpp"
#include "board.hpp"

extern "C"
{
#include "filters/mainsFilter.h"
#include "filters/lowpassFilter.h"
}

mainsFilter current_filter;
mainsFilter voltage_filter;
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
bool IRAM_ATTR get_power(uint16_t *filtered_power)
{
	if (!filter_init)
	{
		filters_init();
	}
	try
	{
		int16_t current = 0;
		int16_t voltage = 0;
		int16_t power = 0;

		mainsFilter_put(&current_filter, analogRead(CURRENT_SENSOR_PIN));
		mainsFilter_put(&voltage_filter, analogRead(VOLTAGE_SENSOR_PIN));

		current = mainsFilter_get(&current_filter);
		voltage = mainsFilter_get(&voltage_filter);
		power = current * voltage;

		lowpassFilter_put(&lowpass_filter, power);
		*filtered_power = lowpassFilter_get(&lowpass_filter);
		return true;
	}
	catch (...)
	{
		Serial.println("Error in get_power");
		return false;
	}
}