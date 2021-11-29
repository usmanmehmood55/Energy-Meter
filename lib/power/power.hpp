/**
 * @file power.hpp
 * @author Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief Used to measure voltage, current and power of a device attached to 
 * the energy meter.
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef POWER_HPP
#define POWER_HPP

#include <Arduino.h>
#include "board.hpp"

/**
 * @brief struct to hold the power data for the current sample.
 */
struct Power
{
	int32_t power = 0;
	int32_t voltage = 0;
	int32_t current = 0;
};

/**
 * @brief Initializes the power measurement filters. 
 * This must be called before using get_power().
 */
void filters_init();


/**
 * @brief This function is called at a frequency of 800Hz. The mains frequency is expected
 * to be around 60 Hz. So 40 - 70 Hz bandpass filters are applied to the voltage and current
 * readings. Instantaneous power is calculated by multiplying the voltage and current.
 * The power is then passed through a low pass filter to get a valid RMS value.
 * 
 * @param filtered_power pointer to struct containing filtered power
 * @return true if calculation was successful
 * @return false if calculation was unsuccessful
 */
bool IRAM_ATTR get_power(Power *filtered_power);

/**
 * @brief prints the power data to the serial port in C, V, P format.
 * 
 * @param filtered_power pointer to variable containing filtered power
 */
void print_power(Power *filtered_power);

#endif