#include "config.h"
#include <Arduino.h>
#include <ServiceController.h>
#include <ModemController.h>

#include <Sensor.h>
#include <SensorDallas.h>
#include <SensorPH.h>
#include <SensorTurbidity.h>

ModemController modem;
ServiceController *service;

void setup()
{
	Serial.begin(115200);
	delay(10);

	service = new ServiceController(modem.getClient());
}

void loop()
{
	
}