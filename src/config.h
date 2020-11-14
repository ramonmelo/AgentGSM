#pragma once

// GPRS Configs

#define OPERATOR_ALGAR
// #define OPERATOR_VIVO

#ifdef OPERATOR_ALGAR
#define APN_ADDR "voxter.br"
#define APN_USER "algar"
#define APN_PASS "algar#"
#endif

#ifdef OPERATOR_VIVO
#define APN_ADDR "m2track.br"
#define APN_USER "vivo"
#define APN_PASS "vivo"
#endif

// MQTT 

// #define MQTT_BROKER "platform.canionlabs.io"
// #define MQTT_ID "68684b80-0102-11eb-9d20-b7806517d2d9"
// #define MQTT_USER "lCvi5DEwExRZpURQckh8"
// #define MQTT_PORT 1883

#define MQTT_BROKER "broker.shiftr.io"
#define MQTT_ID "agent_gsm"
#define MQTT_USER "a502abb6"
#define MQTT_PASS "582a6af067460d36"
#define MQTT_PORT 1883

#define MQTT_DATA_TOPIC "data/topic"