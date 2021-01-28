#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_LOCATION
#define TINY_GSM_TIME
#define TINY_GSM_USE_GPRS

#define SerialMon Serial
#define GSM_AUTOBAUD 9600

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

public:
	ModemController();
	~ModemController();

	bool checkTime(int &year, int &month, int &day, int &hour, int &min, int &sec, float &timezone);

	bool isConnected();

	Client &getClient();
	bool setup();
};

ModemController::ModemController() : SerialAT(2, 3), modem(SerialAT), client(modem) {}

ModemController::~ModemController() {}

Client &ModemController::getClient()
{
	return client;
}

bool ModemController::setup()
{
	SerialAT.begin(GSM_AUTOBAUD);
	delay(6000);

	SerialMon.println(STR_INIT);

	if (modem.init())
	{
		SerialMon.println(STR_SUCCESS);
	}
	else
	{
		return false;
	}

	SerialMon.print(STR_NETWORK);
	if (modem.waitForNetwork() == false)
	{
		SerialMon.println(STR_FAIL);
		return false;
	}

#ifdef DEBUG
	if (modem.isNetworkConnected())
	{
		SerialMon.println(STR_SUCCESS);
	}
#endif

#ifdef TINY_GSM_USE_GPRS
	// GPRS connection parameters are usually set after network registration
	SerialMon.print(STR_CONNECTING);
	SerialMon.print(APN_ADDR);
	if (!modem.gprsConnect(APN_ADDR, APN_USER, APN_PASS))
	{
		SerialMon.println(STR_FAIL);
		return false;
	}

	if (modem.isGprsConnected())
	{
		SerialMon.println(STR_SUCCESS);
	}
#endif

	return true;
}

bool ModemController::isConnected()
{
	return this->modem.isNetworkConnected() && this->modem.isGprsConnected();
}

bool ModemController::checkTime(int &year, int &month, int &day, int &hour, int &min, int &sec, float &timezone)
{
	return modem.getNetworkTime(&year, &month, &day, &hour, &min, &sec, &timezone);
}