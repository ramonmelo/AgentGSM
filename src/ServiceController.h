#define RECONNECT_DELAY 10000L

#include "config.h"
#include <Arduino.h>
#include <PubSubClient.h>

class ServiceController
{
private:
	PubSubClient mqtt;
	unsigned long lastReconnectAttempt = 0;
	unsigned long timeout = 0;

	bool mqttConnect();

public:
	ServiceController(Client &client);
	~ServiceController();

	void setup();
	void service();
	bool sendUpdate(const char *data);
	bool sendUpdate(const char* topic, const uint8_t* payload, unsigned int plength);
	bool isConnected();
};

ServiceController::ServiceController(Client &client) : mqtt(client)
{
}

ServiceController::~ServiceController()
{
}

void ServiceController::setup()
{
	// MQTT Broker setup
	mqtt.setServer(MQTT_BROKER, MQTT_PORT);
	// mqtt.setCallback(mqttCallback);
}

void ServiceController::service()
{
	if (mqtt.connected() == false)
	{
		Serial.println(CODE_M1);
		// Reconnect every 10 seconds
		uint32_t t = millis();
		if (t - lastReconnectAttempt > RECONNECT_DELAY)
		{
			lastReconnectAttempt = t;
			if (mqttConnect())
			{
				lastReconnectAttempt = 0;
			}
		}
		delay(100);
		return;
	}

	mqtt.loop();
}

bool ServiceController::isConnected()
{
	return mqtt.connected();
}

// void mqttCallback(char *topic, byte *payload, unsigned int len)
// {
// 	SerialMon.print("Message arrived [");
// 	SerialMon.print(topic);
// 	SerialMon.print("]: ");
// 	SerialMon.write(payload, len);
// 	SerialMon.println();
// 	// Only proceed if incoming message's topic matches
// 	if (String(topic) == topicLed)
// 	{
// 		ledStatus = !ledStatus;
// 		digitalWrite(LED_PIN, ledStatus);
// 		mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
// 	}
// }

boolean ServiceController::mqttConnect()
{
	Serial.print(STR_CONNECTING);
	Serial.print(MQTT_BROKER);

	// Connect to MQTT Broker
	boolean status = mqtt.connect(MQTT_ID, MQTT_USER, MQTT_PASS);

	if (status == false)
	{
		Serial.println(STR_FAIL);
		return false;
	}
	Serial.println(STR_SUCCESS);

	return mqtt.connected();
}

bool ServiceController::sendUpdate(const char *data)
{
	return mqtt.connected() && mqtt.publish(MQTT_DATA_TOPIC, data);
}

bool ServiceController::sendUpdate(const char* topic, const uint8_t* payload, unsigned int plength)
{
	return mqtt.connected() && mqtt.publish(topic, payload, plength);
}