#pragma once

#include <Arduino.h>
#include <Sensor.h>

namespace smartaqua
{
    class SensorPH : public Sensor
    {
    private:
        int analogPin;
        int temperature = 25;

    public:
        SensorPH(int pin);
        ~SensorPH();

        void service() override;
        char prefix() override;
    };

    SensorPH::SensorPH(int pin) : analogPin(pin)
    {
        pinMode(analogPin, INPUT);
        initData(1);
    }

    SensorPH::~SensorPH()
    {
    }

    void SensorPH::service()
    {
        setData(0, readAsAnalog(analogPin));
    }

    char SensorPH::prefix()
    {
        return 'p';
    }

} // namespace smartaqua