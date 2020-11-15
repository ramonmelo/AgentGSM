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

// #define MQTT_BROKER "platform.canionlabs.io"
// #define MQTT_ID "68684b80-0102-11eb-9d20-b7806517d2d9"
// #define MQTT_USER "lCvi5DEwExRZpURQckh8"
// #define MQTT_PORT 1883

const char *MQTT_BROKER("smartacqua.cloud.shiftr.io");
const char *MQTT_ID("acqua001");
const char *MQTT_USER("smartacqua");
const char *MQTT_PASS("jO8zkKzCLktGQfT0");
const int MQTT_PORT = 1883;

const char *MQTT_DATA_TOPIC("dt/topic");
const char *MQTT_DATA_TOPIC_TEMPERATURE("dt/tp"); // data/temperature

// Text Area

const String STR_FAIL(" fl");        // fail
const String STR_SUCCESS(" sc");     // success
const String STR_CONNECTING("ctg "); // connecting
const String STR_INIT("it...");      // init
const String STR_NETWORK("nt...");   // network

// Codes:
const String CODE_M1("M1"); // MQTT NOT CONNECTED
const String CODE_M2("M2"); // MODEM NOT CONNECTED