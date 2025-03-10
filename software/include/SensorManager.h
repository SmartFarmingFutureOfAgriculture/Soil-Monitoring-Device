#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

class SensorManager {
public:
    SensorManager();
    String getSensorDataJson();
};

#endif