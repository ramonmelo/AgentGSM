#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_LOCATION
#define TINY_GSM_TIME
#define TINY_GSM_USE_GPRS

#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon

#define GSM_AUTOBAUD 9600
#define GSM_PIN ""

#include "config.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGsmClient.h>

class ModemController
{
private:
	SoftwareSerial SerialAT;
	TinyGsm modem;
	TinyGsmClient client;

	void checkPosition();
	void checkTime();

public:
	ModemController();
	~ModemController();

	Client& getClient();
	bool setup();
};

ModemController::ModemController() : SerialAT(12, 13), modem(SerialAT), client(modem)
{
}

ModemController::~ModemController()
{
}

Client& ModemController::getClient()
{
	return client;
}

bool ModemController::setup()
{
	SerialAT.begin(GSM_AUTOBAUD);
	delay(6000);

	SerialMon.println("Initializing modem...");
	// modem.restart();
	modem.init();

#ifdef DEBUG
	String modemInfo = modem.getModemInfo();
	SerialMon.print("Modem Info: ");
	SerialMon.println(modemInfo);
#endif

#ifdef TINY_GSM_USE_GPRS
	// Unlock your SIM card with a PIN if needed
	if (GSM_PIN && modem.getSimStatus() != 3)
	{
		modem.simUnlock(GSM_PIN);
	}
#endif

	SerialMon.print("Waiting for network...");
	if (!modem.waitForNetwork())
	{
		SerialMon.println(" fail");
		delay(10000);

		return false;
	}
	SerialMon.println(" success");

	if (modem.isNetworkConnected())
	{
		SerialMon.println("Network connected");
	}

#ifdef TINY_GSM_USE_GPRS
	// GPRS connection parameters are usually set after network registration
	SerialMon.print(F("Connecting to "));
	SerialMon.print(APN_ADDR);
	if (!modem.gprsConnect(APN_ADDR, APN_USER, APN_PASS))
	{
		SerialMon.println(" fail");
		delay(10000);

		return false;
	}
	SerialMon.println(" success");

	if (modem.isGprsConnected())
	{
		SerialMon.println("GPRS connected");
	}

	// bool res = modem.isGprsConnected();
	// DBG("GPRS status:", res ? "connected" : "not connected");

	// String ccid = modem.getSimCCID();
	// DBG("CCID:", ccid);

	// String imei = modem.getIMEI();
	// DBG("IMEI:", imei);

	// String imsi = modem.getIMSI();
	// DBG("IMSI:", imsi);

	// String cop = modem.getOperator();
	// DBG("Operator:", cop);

	// IPAddress local = modem.localIP();
	// DBG("Local IP:", local);

	// int csq = modem.getSignalQuality();
	// DBG("Signal quality:", csq);
#endif

	return true;
}

void ModemController::checkPosition()
{
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
}

void ModemController::checkTime()
{
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
}