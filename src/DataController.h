#include <Arduino.h>
#include <ArduinoJson.h>

class DataController
{
private:
    float temp1;
    float ph;
    float turb;

public:
    DataController();
    ~DataController();

    void setData(float temp1, float ph, float turb);
    void BuildInfo(char* msg, int len);
};

DataController::DataController()
{
}

DataController::~DataController()
{
}

void DataController::setData(float temp1, float ph, float turb)
{
    this->temp1 = temp1;
    this->ph = ph;
    this->turb = turb;
}

void DataController::BuildInfo(char* msg, int len)
{
    StaticJsonDocument<24> doc;

    doc["t1"] = this->temp1;
    doc["p"] = this->ph;
    doc["b"] = this->turb;

    serializeJson(doc, msg, len);
}
