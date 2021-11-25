#include <Arduino.h>
#include "power.hpp"
#include "board.hpp"

hw_timer_t *timer = NULL;
void setup()
{
	Serial.begin(115200);
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &timerCallback, true);
	timerAlarmWrite(timer, 5000, true);
	timerAlarmEnable(timer);
	pinMode(CURRENT_SENSOR_PIN, INPUT);
	pinMode(VOLTAGE_SENSOR_PIN, INPUT);
	
	setup_filter();

	Serial.println("App Started");
}

void loop()
{
	// do nothing
}

void IRAM_ATTR timerCallback()
{
	uint16_t filtered_power = 0;
	if(get_power(&filtered_power))
	{
		Serial.print("Filtered Power: ");
		Serial.println(filtered_power);
	}
}