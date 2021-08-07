#pragma once

#include <Arduino.h>
#include <Sensor.h>
#include <math.h>

namespace smartaqua
{
    class SensorTurbidity : public Sensor
    {
    private:
        int analogPin;
        const char prefix_id = 'b';

    public:
        SensorTurbidity(int pin);
        ~SensorTurbidity();

        bool service() override;
        char prefix() override;
        bool setup() override;
    };

    SensorTurbidity::SensorTurbidity(int pin) : analogPin(pin) {}
    SensorTurbidity::~SensorTurbidity() {}

    bool SensorTurbidity::service()
    {
        setData(0, readAsAnalog(analogPin));

        return true;
    }

    char SensorTurbidity::prefix()
    {
        return this->prefix_id;
    }

    bool SensorTurbidity::setup()
    {
        pinMode(analogPin, INPUT);
        initData(1);

        return true;
    }

} // namespace smartaqua