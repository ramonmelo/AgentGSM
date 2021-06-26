#include <Arduino.h>
#include "config.h"
#include <ServiceController.h>
#include <ModemController.h>
#include <DataController.h>

#include <Sensor.h>
#include <SensorDallas.h>
#include <SensorPH.h>
#include <SensorTurbidity.h>

#define ONE_WIRE_BUS 5
#define PIN_SENSOR_PH A1
#define PIN_SENSOR_TURBIDITY A0

ModemController modem;
ServiceController *service;
DataController dataController;

OneWire oneWire(ONE_WIRE_BUS);

smartaqua::SensorDallas sensorDallas(&oneWire);
smartaqua::SensorPH sensorPH(PIN_SENSOR_PH);
smartaqua::SensorTurbidity sensorTurbidity(PIN_SENSOR_TURBIDITY);

// Reset Method
void (*resetFunc)(void) = 0;

// Timer
unsigned long nextUpdate;
unsigned const long UPDATE_DELAY = 1000l * 5l;

// Json
int8_t msg_size = 50;
char *msg;

void setupSensor(smartaqua::Sensor &sensor)
{
	if (sensor.setup())
	{
		Serial.println(STR_SUCCESS);
	}
}

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

	setupSensor(sensorDallas);
	setupSensor(sensorPH);
	setupSensor(sensorTurbidity);

	msg = new char[msg_size];
}

void sendData(const char *topic, float data)
{
	String dataString(data);
	service->sendUpdate(topic, dataString.c_str());
}

void update()
{
	if (service->isConnected() && nextUpdate < millis())
	{
		dataController.setData(
				sensorDallas.read(0),
				sensorPH.read(0),
				sensorTurbidity.read(0));

		dataController.BuildInfo(msg, msg_size);

		service->sendUpdate(MQTT_DATA_TOPIC, msg);

		nextUpdate = millis() + UPDATE_DELAY;
	}
}

void loop()
{
	if (modem.isConnected())
	{
		service->service();

		sensorDallas.service();
		sensorPH.service();
		sensorTurbidity.service();

		update();
	}
	else
	{
		Serial.println(CODE_M2);
		resetFunc();
	}
}