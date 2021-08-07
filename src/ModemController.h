#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_LOCATION
#define TINY_GSM_TIME

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

	void shutdown();
	bool checkTime(int &year, int &month, int &day, int &hour, int &min, int &sec, float &timezone);

	bool isConnected();

	Client &getClient();
	bool setup();
};

ModemController::ModemController() : SerialAT(2, 3), modem(SerialAT), client(modem) {}

ModemController::~ModemController() {}

void ModemController::shutdown()
{
	modem.gprsDisconnect();
	delay(5000L);
	modem.poweroff();
}

Client &ModemController::getClient()
{
	return client;
}

bool ModemController::setup()
{
	SerialAT.begin(GSM_AUTOBAUD);
	delay(6000);

	// Init Modem
	SerialMon.println(STR_INIT);
	if (modem.init())
	{
		String modemInfo = modem.getModemInfo();
		SerialMon.print("Info: ");
		SerialMon.println(modemInfo);

		// Connect to Mobile Network
		SerialMon.println(STR_NETWORK);
		if (modem.waitForNetwork() && modem.isNetworkConnected())
		{
			// Connect to GPRS
			SerialMon.print(STR_CONNECTING);
			SerialMon.println(APN_ADDR);
			if (modem.gprsConnect(APN_ADDR, APN_USER, APN_PASS) && modem.isGprsConnected())
			{
				SerialMon.println(modem.getOperator());
				return true;
			}
		}
	}

	SerialMon.println(STR_FAIL);
	return false;
}

bool ModemController::isConnected()
{
	return this->modem.isNetworkConnected() && this->modem.isGprsConnected();
}

bool ModemController::checkTime(int &year, int &month, int &day, int &hour, int &min, int &sec, float &timezone)
{
	return modem.getNetworkTime(&year, &month, &day, &hour, &min, &sec, &timezone);
}