#include <Arduino.h>
#include "config.h"
#include <ArduinoJson.h>
#include <ServiceController.h>
#include <ModemController.h>

#include <Sensor.h>
#include <SensorDallas.h>
#include <SensorPH.h>
#include <SensorTurbidity.h>

#define UPLOAD_DELAY 1000 * 5
#define ONE_WIRE_BUS 10
#define PIN_SENSOR_PH A1
#define PIN_SENSOR_TURBIDITY A0

ModemController modem;
ServiceController *service;

OneWire oneWire(ONE_WIRE_BUS);

smartaqua::SensorDallas sensorDallas(&oneWire);
smartaqua::SensorPH sensorPH(PIN_SENSOR_PH);
smartaqua::SensorTurbidity sensorTurbidity(PIN_SENSOR_TURBIDITY);

// Reset Method
void (*resetFunc)(void) = 0;

// Timer
unsigned long nextUpdate;

// Json
const char* KEY_D = "d";
const char* KEY_S = "s";
uint8_t msg[50];

void setup()
{
	Serial.begin(115200);
	delay(10);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	if (modem.setup())
	{
		service = new ServiceController(modem.getClient());
		service->setup();
	}
	else
	{
		Serial.println(STR_FAIL);
		delay(10000);

		resetFunc();
	}

	nextUpdate = millis();
}

void update()
{
	if (service->isConnected() && nextUpdate < millis())
	{
		const size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(2);

		DynamicJsonDocument doc(capacity);

		JsonArray d = doc.createNestedArray(KEY_D);
		d.add(1000);
		d.add(2000);
		d.add(3000);
		d.add(4000);
		doc[KEY_S] = 1351824120;

		unsigned int total = serializeJson(doc, msg);

		service->sendUpdate(MQTT_DATA_TOPIC, msg, total);

		nextUpdate = millis() + UPLOAD_DELAY;
	}
}

void loop()
{
	if (modem.isConnected())
	{
		service->service();
		update();
	}
	else
	{
		Serial.println(CODE_M2);
		resetFunc();
	}
}