#pragma once

#include <Arduino.h>
#include <Sensor.h>

namespace smartaqua
{
    class SensorPH : public Sensor
    {
    private:
        int analogPin;
        const char prefix_id = 'p';

    public:
        SensorPH(int pin);
        ~SensorPH();

        bool service() override;
        char prefix() override;
        bool setup() override;
    };

    SensorPH::SensorPH(int pin) : analogPin(pin) {}
    SensorPH::~SensorPH() {}

    bool SensorPH::service()
    {
        setData(0, readAsAnalog(analogPin));

        return true;
    }

    char SensorPH::prefix()
    {
        return this->prefix_id;
    }

    bool SensorPH::setup()
    {
        pinMode(analogPin, INPUT);
        initData(1);

        return true;
    }

} // namespace smartaqua