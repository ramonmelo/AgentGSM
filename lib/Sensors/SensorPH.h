#pragma once

#include <Arduino.h>
#include <Sensor.h>

namespace smartaqua
{
    class SensorPH : public Sensor
    {
    private:
    public:
        SensorPH();
        ~SensorPH();

        void service() override;
        int length() override;
        char prefix() override;
    };

    SensorPH::SensorPH()
    {
    }

    SensorPH::~SensorPH()
    {
    }

    void SensorPH::service()
    {

    }

    int SensorPH::length()
    {
        return 1;
    }

    char SensorPH::prefix()
    {
        return 'p';
    }

} // namespace smartaqua