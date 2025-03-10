#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>

class WiFiManager {
public:
    WiFiManager(const char *ssid, const char *password, WiFiClientSecure &client);
    void connect();
    
private:
    const char *ssid;
    const char *password;
    WiFiClientSecure &client;
};

#endif