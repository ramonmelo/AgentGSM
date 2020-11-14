#pragma once

#include "Arduino.h"
#include <limits.h>

#define N_AVERAGE 800

namespace smartaqua
{
    class Sensor
    {
    public:
        Sensor();
        ~Sensor();

        virtual void service() = 0;
        virtual char prefix() = 0;

        float read(int idx);
        void setData(int pin, float data);
        void initData(int size);
        int length();

    protected:
        float readAsAnalog(int pin);
        int readAsDigital(int pin);

    private:
        float *data;
        int size;
    };

    Sensor::Sensor()
    {
    }

    Sensor::~Sensor()
    {
        delete[] data;
    }

    void Sensor::initData(int size)
    {
        this->size = size;
        this->data = new float[size];
    }

    float Sensor::read(int index)
    {
        if (index >= 0 && index < this->size)
        {
            return data[index];
        }

        return INT_MIN;
    }

    void Sensor::setData(int index, float value)
    {
        if (index >= 0 && index < this->size)
        {
            this->data[index] = value;
        }
    }

    int Sensor::length()
    {
        return this->size;
    }

    float Sensor::readAsAnalog(int pin)
    {
        float reading = 0;

        for (size_t i = 0; i < N_AVERAGE; i++)
        {
            reading += ((float)analogRead(pin) / 1023);
        }

        reading = (reading / N_AVERAGE) * 5000;

        return reading;
    }

    int Sensor::readAsDigital(int pin)
    {
        return digitalRead(pin);
    }

} // namespace smartaqua