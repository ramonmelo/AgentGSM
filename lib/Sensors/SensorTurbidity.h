#pragma once

#include <Arduino.h>
#include <Sensor.h>
#include <math.h>

#define N_AVERAGE 800

namespace smartaqua
{
    class SensorTurbidity : public Sensor
    {
    private:
        int analogPin;

    public:
        SensorTurbidity(int pin);
        ~SensorTurbidity();

        void service() override;
        int length() override;
        char prefix() override;
    };

    SensorTurbidity::SensorTurbidity(int pin) : analogPin(pin)
    {
        pinMode(analogPin, INPUT);
    }

    SensorTurbidity::~SensorTurbidity()
    {
    }

    void SensorTurbidity::service()
    {
        float NTU = 0;
        float reading = 0;

        for (size_t i = 0; i < N_AVERAGE; i++)
        {
            reading += ((float)analogRead(reading) / (1023)) * 5;
        }

        reading = reading / N_AVERAGE;
        reading = roundf(reading);

        if (reading < 2.5)
        {
            NTU = 3000;
        }
        else if (reading > 4.2)
        {
            NTU = 0;
        }
        else
        {
            NTU = (-1120.4 * reading * reading) + (5742.3 * reading) - 4353.8;
        }

        setData(0, NTU);
    }

    int SensorTurbidity::length()
    {
        return 1;
    }

    char SensorTurbidity::prefix()
    {
        return 'b';
    }

} // namespace smartaqua