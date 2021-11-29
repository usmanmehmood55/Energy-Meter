#include <Arduino.h>
#include "power.hpp"
#include "board.hpp"

hw_timer_t *timer = NULL;
TaskHandle_t core_1_task;
bool perform_core_1_task = false;

void IRAM_ATTR timerCallback()
{
	perform_core_1_task = true;
}

void core_1_body(void *pvParameters)
{
	Serial.print("core_1_task running on core ");
	Serial.println(xPortGetCoreID());
	Power filtered_power;
	while (1)
	{
		Serial.print("");
		if (perform_core_1_task)
		{
			perform_core_1_task = false;
			get_power(&filtered_power);
			print_power(&filtered_power);
		}
	}
}

void setup()
{
	Serial.begin(115200);

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &timerCallback, true);
	timerAlarmWrite(timer, 1600, true);
	timerAlarmEnable(timer);

	xTaskCreatePinnedToCore(
		core_1_body,  /* Task function. */
		"Core 1",	  /* name of task. */
		10000,		  /* Stack size of task */
		NULL,		  /* parameter of the task */
		1,			  /* priority of the task */
		&core_1_task, /* Task handle to keep track of created task */
		1);			  /* pin task to core 1 */

	pinMode(CURRENT_SENSOR_PIN, INPUT);
	pinMode(VOLTAGE_SENSOR_PIN, INPUT);
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);

	filters_init();

	Serial.println("App Started");
}

void loop()
{
	// do nothing
}