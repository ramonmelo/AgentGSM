#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <Sensor.h>

#define INVALID_VALUE -127

namespace smartaqua
{
    class SensorDallas : public Sensor
    {
    private:
        DallasTemperature manager;
        DeviceAddress *sensors;
        int sensor_count;

        const char prefix_id = 't';

    public:
        SensorDallas(OneWire *ow);
        ~SensorDallas();

        void service() override;
        char prefix() override;
        bool setup() override;
    };

    SensorDallas::SensorDallas(OneWire *ow) : manager(ow) {}
    SensorDallas::~SensorDallas() {}

    void SensorDallas::service()
    {
        manager.requestTemperatures();

        for (int i = 0; i < sensor_count; ++i)
        {
            float current_temp = manager.getTempC(sensors[i]);

            if (current_temp != INVALID_VALUE)
            {
                setData(i, current_temp);
            }
        }
    }

    char SensorDallas::prefix()
    {
        return this->prefix_id;
    }

    bool SensorDallas::setup()
    {
        manager.begin();
        sensor_count = manager.getDeviceCount();

        Serial.print("dl ");
        Serial.println(sensor_count, DEC);

        sensors = new DeviceAddress[sensor_count];

        initData(sensor_count);

        for (int i = 0; i < sensor_count; ++i)
        {
            if (manager.getAddress(sensors[i], i))
            {
                manager.setResolution(sensors[i], 9);
            }

            setData(i, 0);
        }

        return sensor_count > 0;
    }

} // namespace smartaqua