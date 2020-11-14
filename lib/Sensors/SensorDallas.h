#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <Sensor.h>

#define INVALID_VALUE -127

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
        {
            Serial.print("0");
        }
        Serial.print(deviceAddress[i], HEX);
    }
}

namespace smartaqua
{
    class SensorDallas : public Sensor
    {
    private:
        DallasTemperature manager;
        DeviceAddress *sensors;
        int sensor_count;

        int temp_min;
        int temp_max;
        bool alert_temp_change;

    public:
        SensorDallas(OneWire *ow);
        ~SensorDallas();

        void service() override;
        int length() override;
        char prefix() override;
    };

    SensorDallas::SensorDallas(OneWire *ow) : manager(ow), alert_temp_change(false)
    {
        Serial.print("Locating devices...");
        manager.begin();
        sensor_count = manager.getDeviceCount();

        Serial.print("Found ");
        Serial.print(sensor_count, DEC);
        Serial.println(" devices.");

        Serial.print("Parasite power is: ");
        if (manager.isParasitePowerMode())
        {
            Serial.println("ON");
        }
        else
        {
            Serial.println("OFF");
        }

        sensors = new DeviceAddress[sensor_count];

        for (int i = 0; i < sensor_count; ++i)
        {
            if (manager.getAddress(sensors[i], i))
            {
                Serial.println("Found Device");
                manager.setResolution(sensors[i], 9);
            }
            else
            {
                Serial.println("Unable to find address for Device");
            }

            setData(i, 0);
        }
    }

    SensorDallas::~SensorDallas()
    {
    }

    void SensorDallas::service()
    {
        manager.requestTemperatures();

        for (int i = 0; i < sensor_count; ++i)
        {
            float current_temp = manager.getTempC(sensors[i]);

            if (current_temp == INVALID_VALUE)
            {
                current_temp = read(i);
            }

            setData(i, current_temp);
        }
    }

    int SensorDallas::length()
    {
        return sensor_count;
    }

    char SensorDallas::prefix()
    {
        return 't';
    }

} // namespace smartaqua