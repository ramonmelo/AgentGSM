#define RECONNECT_DELAY 10000L
#define UPDATE_DELAY 15 * 1000

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
	bool sendUpdate(char* data);
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
	if (!mqtt.connected())
	{
		Serial.println("=== MQTT NOT CONNECTED ===");
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
	Serial.print("Connecting to ");
	Serial.print(MQTT_BROKER);

	// Connect to MQTT Broker
	boolean status = mqtt.connect(MQTT_ID, MQTT_USER, MQTT_PASS);

	if (status == false)
	{
		Serial.println(" fail");
		return false;
	}
	Serial.println(" success");

	return mqtt.connected();
}

bool ServiceController::sendUpdate(char* data)
{
	return mqtt.connected() && mqtt.publish(MQTT_DATA_TOPIC, data);
}