#include <config.h>

// Select your modem:
#define TINY_GSM_MODEM_SIM800

#define TINY_GSM_TEST_GSM_LOCATION true
#define TINY_GSM_TEST_TIME true

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
// #define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(D2, D1); // RX, TX

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Add a reception delay - may be needed for a fast processor at a slow baud rate
// #define TINY_GSM_YIELD() { delay(2); }

// Define how you're planning to connect to the internet
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

const char *topicLed = "rmGsmClientTest/led";
const char *topicInit = "rmGsmClientTest/init";
const char *topicLedStatus = "rmGsmClientTest/ledStatus";

#include <TinyGsmClient.h>
#include <PubSubClient.h>

// Just in case someone defined the wrong thing..
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define LED_PIN 2
int ledStatus = LOW;

uint32_t lastReconnectAttempt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
	SerialMon.print("Message arrived [");
	SerialMon.print(topic);
	SerialMon.print("]: ");
	SerialMon.write(payload, len);
	SerialMon.println();

	// Only proceed if incoming message's topic matches
	if (String(topic) == topicLed)
	{
		ledStatus = !ledStatus;
		digitalWrite(LED_PIN, ledStatus);
		mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
	}
}

boolean mqttConnect()
{
	SerialMon.print("Connecting to ");
	SerialMon.print(MQTT_BROKER);

	// Connect to MQTT Broker
	boolean status = mqtt.connect(MQTT_ID, MQTT_USER, MQTT_PASS);

	// Or, if you want to authenticate MQTT:
	// boolean status = mqtt.connect("GsmClientName", mqtt_user, mqtt_pass);

	if (status == false)
	{
		SerialMon.println(" fail");
		return false;
	}
	SerialMon.println(" success");
	mqtt.publish(topicInit, "GsmClientTest started");
	mqtt.subscribe(topicLed);
	return mqtt.connected();
}

void checkPosition()
{
	// if (mqtt.connected())
	// {
	// }

#if TINY_GSM_TEST_GSM_LOCATION && defined TINY_GSM_MODEM_HAS_GSM_LOCATION
	float lat = 0;
	float lon = 0;
	float accuracy = 0;
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	for (int8_t i = 15; i; i--)
	{
		DBG("Requesting current GSM location");
		if (modem.getGsmLocation(&lat, &lon, &accuracy, &year, &month, &day, &hour,
								 &min, &sec))
		{
			DBG("Latitude:", String(lat, 8), "\tLongitude:", String(lon, 8));
			DBG("Accuracy:", accuracy);
			DBG("Year:", year, "\tMonth:", month, "\tDay:", day);
			DBG("Hour:", hour, "\tMinute:", min, "\tSecond:", sec);
			break;
		}
		else
		{
			DBG("Couldn't get GSM location, retrying in 15s.");
			delay(15000L);
		}
	}
	DBG("Retrieving GSM location again as a string");
	String location = modem.getGsmLocation();
	DBG("GSM Based Location String:", location);
#endif
}

void checkTime()
{
#if TINY_GSM_TEST_TIME && defined TINY_GSM_MODEM_HAS_TIME
	int year3 = 0;
	int month3 = 0;
	int day3 = 0;
	int hour3 = 0;
	int min3 = 0;
	int sec3 = 0;
	float timezone = 0;
	for (int8_t i = 5; i; i--)
	{
		DBG("Requesting current network time");
		if (modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3,
								 &timezone))
		{
			DBG("Year:", year3, "\tMonth:", month3, "\tDay:", day3);
			DBG("Hour:", hour3, "\tMinute:", min3, "\tSecond:", sec3);
			DBG("Timezone:", timezone);
			break;
		}
		else
		{
			DBG("Couldn't get network time, retrying in 15s.");
			delay(15000L);
		}
	}
	DBG("Retrieving time again as a string");
	String time = modem.getGSMDateTime(DATE_FULL);
	DBG("Current Network Time:", time);
#endif
}

void setup()
{
	// Set console baud rate
	SerialMon.begin(115200);
	delay(10);

	pinMode(LED_PIN, OUTPUT);

	// !!!!!!!!!!!
	// Set your reset, enable, power pins here
	// !!!!!!!!!!!

	SerialMon.println("Wait...");

	// Set GSM module baud rate
	TinyGsmAutoBaud(SerialAT, 9600, 9600);
	SerialAT.begin(9600);
	delay(6000);

	// Restart takes quite some time
	// To skip it, call init() instead of restart()
	SerialMon.println("Initializing modem...");
	// modem.restart();
	modem.init();

	String modemInfo = modem.getModemInfo();
	SerialMon.print("Modem Info: ");
	SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
	// Unlock your SIM card with a PIN if needed
	if (GSM_PIN && modem.getSimStatus() != 3)
	{
		modem.simUnlock(GSM_PIN);
	}
#endif

#if TINY_GSM_USE_WIFI
	// Wifi connection parameters must be set before waiting for the network
	SerialMon.print(F("Setting SSID/password..."));
	if (!modem.networkConnect(wifiSSID, wifiPass))
	{
		SerialMon.println(" fail");
		delay(10000);
		return;
	}
	SerialMon.println(" success");
#endif

	SerialMon.print("Waiting for network...");
	if (!modem.waitForNetwork())
	{
		SerialMon.println(" fail");
		delay(10000);

		ESP.restart();

		return;
	}
	SerialMon.println(" success");

	if (modem.isNetworkConnected())
	{
		SerialMon.println("Network connected");
	}

#if TINY_GSM_USE_GPRS
	// GPRS connection parameters are usually set after network registration
	SerialMon.print(F("Connecting to "));
	SerialMon.print(APN_ADDR);
	if (!modem.gprsConnect(APN_ADDR, APN_USER, APN_PASS))
	{
		SerialMon.println(" fail");
		delay(10000);

		ESP.restart();

		return;
	}
	SerialMon.println(" success");

	if (modem.isGprsConnected())
	{
		SerialMon.println("GPRS connected");
	}

	bool res = modem.isGprsConnected();
	DBG("GPRS status:", res ? "connected" : "not connected");

	String ccid = modem.getSimCCID();
	DBG("CCID:", ccid);

	String imei = modem.getIMEI();
	DBG("IMEI:", imei);

	String imsi = modem.getIMSI();
	DBG("IMSI:", imsi);

	String cop = modem.getOperator();
	DBG("Operator:", cop);

	IPAddress local = modem.localIP();
	DBG("Local IP:", local);

	int csq = modem.getSignalQuality();
	DBG("Signal quality:", csq);
#endif

	checkTime();
	checkPosition();

	// MQTT Broker setup
	mqtt.setServer(MQTT_BROKER, MQTT_PORT);
	mqtt.setCallback(mqttCallback);
}

unsigned long timeout = 0;

void loop()
{

	if (!mqtt.connected())
	{
		SerialMon.println("=== MQTT NOT CONNECTED ===");
		// Reconnect every 10 seconds
		uint32_t t = millis();
		if (t - lastReconnectAttempt > 10000L)
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

	// if ((millis() - timeout) > (15 * 1000))
	// {
	// 	DBG("Timeout: ", timeout);
	// 	sendPosition();

	// 	timeout = millis();
	// }

	mqtt.loop();
}