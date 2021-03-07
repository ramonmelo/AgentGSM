# Agent GSM

This agent is based on a GPRS connection in order to communicate with external services

## Currently Used Libraries

- PlatformIO: [https://platformio.org/]
- TinyGSM: [https://github.com/vshymanskyy/TinyGSM]
- PubSubClient: [https://github.com/knolleary/pubsubclient]

## Links

- [https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial/]
- [https://github.com/vshymanskyy/TinyGSM]
- [https://www.instructables.com/SMS-Controller-by-Using-SIM800L-V2/]
- [https://andreasrohner.at/posts/Electronics/How-to-modify-an-Arduino-Pro-Mini-clone-for-low-power-consumption/]
- [https://github.com/rocketscream/Low-Power]

## Sensor Links

1. PH
   1. [https://wiki.dfrobot.com/Gravity__Analog_pH_Sensor_Meter_Kit_V2_SKU_SEN0161-V2]
   2. [https://cimpleo.com/blog/simple-arduino-ph-meter/]
   3. [https://scidle.com/how-to-use-a-ph-sensor-with-arduino/]
   4. [https://www.electroniclinic.com/ph-meter-arduino-ph-meter-calibration-diymore-ph-sensor-arduino-code/]
   5. [https://how2electronics.com/ph-meter-using-ph-sensor-arduino-oled/]
   6. [https://www.electroniclinic.com/ph-sensor-arduino-how-do-ph-sensors-work-application-of-ph-meter-ph-sensor-calibration/]
   7. [https://www.marinostore.com/sensores/sensores/sensor-de-ph-com-modulo-ph4502c]
2. Turbidity
   1. [https://www.usinainfo.com.br/blog/sensor-de-turbidez-projeto-de-leitura-da-qualidade-da-agua/]
   2. [https://forum.arduino.cc/index.php?topic=398900.0]

## PH Convertion Code

```js
var ph_7 = parseFloat(metadata.ss_ph_7); // 2500
var ph_4 = parseFloat(metadata.ss_ph_4); // 4000

var slope = (7.0 - 4.0) / (ph_7 - ph_4);
var ph = slope * ( msg.p - ph_7 ) + 7.0;

msg.PH = ph;

return {msg: msg, metadata: metadata, msgType: msgType};
```

## Turbidity Convertion COde

```js
var x = parseFloat(msg.b) / 1000.0;
var NTU = 0;

if (x < 2.5) // Min Voltage
{
    NTU = 3000;
}
else if (x > 4.2) // MAX Voltage
{
    NTU = 0;
}
else
{
    NTU = (-1120.4 * x * x) + (5742.3 * x) - 4352.9;
}

msg.NTU = NTU;

return {msg: msg, metadata: metadata, msgType: msgType};
```
