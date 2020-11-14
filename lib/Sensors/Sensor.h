#pragma once

#include <map>
#include "Arduino.h"

namespace smartaqua
{
    class Sensor
    {
    public:
        Sensor();

        virtual void service() = 0;
        virtual int length() = 0;
        virtual char prefix() = 0;

        float read(int idx);
        void setData(int pin, float data);

    protected:
        std::map<int, float> info;
    };

    Sensor::Sensor() {}

    float Sensor::read(int idx)
    {
        return 0;
    }

    void Sensor::setData(int pin, float data)
    {
        info[pin] = data;
    }

} // namespace smartaqua
