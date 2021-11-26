#ifndef POWER_HPP
#define POWER_HPP

#include <Arduino.h>

/**
 * @brief Initializes the power measurement filters. 
 * This must be called before using get_power().
 */
void filters_init();

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
bool get_power(uint16_t *filtered_power);

#endif