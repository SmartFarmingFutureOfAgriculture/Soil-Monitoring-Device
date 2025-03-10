#ifndef DATA_SENDER_H
#define DATA_SENDER_H

#include <WiFiClientSecure.h>

class DataSender {
public:
    DataSender(WiFiClientSecure &client);
    void sendSensorData(const String &jsonPayload);

private:
    WiFiClientSecure &client;
};

#endif