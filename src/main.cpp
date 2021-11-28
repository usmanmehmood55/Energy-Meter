#include <Arduino.h>
#include "power.hpp"
#include "board.hpp"

hw_timer_t *timer = NULL;
void IRAM_ATTR timerCallback()
{
	int32_t filtered_power = 0;
	get_power(&filtered_power);
}

void setup()
{
	Serial.begin(115200);
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &timerCallback, true);
	timerAlarmWrite(timer, 2000, true);
	timerAlarmEnable(timer);
	pinMode(CURRENT_SENSOR_PIN, INPUT);
	pinMode(VOLTAGE_SENSOR_PIN, INPUT);
	pinMode(RELAY_PIN, OUTPUT);

	filters_init();

	Serial.println("App Started");
}

void loop()
{
	digitalWrite(RELAY_PIN, LOW);
}