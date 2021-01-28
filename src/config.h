#pragma once

// GPRS Configs

#define OPERATOR_ALGAR
// #define OPERATOR_VIVO

#ifdef OPERATOR_ALGAR
const char *APN_ADDR("voxter.br");
const char *APN_USER("algar");
const char *APN_PASS("algar#");
#endif

#ifdef OPERATOR_VIVO
const char *APN_ADDR("m2track.br");
const char *APN_USER("vivo");
const char *APN_PASS("vivo");
#endif

// MQTT

const char *MQTT_BROKER("");
const char *MQTT_ID("");
const char *MQTT_USER("");
const char *MQTT_PASS("");
const int MQTT_PORT = 1883;

const char *MQTT_DATA_TOPIC("v1/devices/me/telemetry");

// Text Area

const String STR_FAIL(" fl");        // fail
const String STR_SUCCESS(" sc");     // success
const String STR_CONNECTING("ctg "); // connecting
const String STR_INIT("it...");      // init
const String STR_NETWORK("nt...");   // network

// Codes:
const String CODE_M1("M1"); // MQTT NOT CONNECTED
const String CODE_M2("M2"); // MODEM NOT CONNECTED