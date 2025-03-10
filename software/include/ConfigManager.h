/*
 * Smart Farming Soil Monitoring System
 * Copyright (C) 2024 Smart Farming gGmbH
 *
 * This software is released under the GNU General Public License v2 (GPL-2.0).
 * You are free to use, modify, and distribute it under the terms of this license.
 *
 * NO WARRANTY: This software is provided "as is", without any warranty of any kind.
 * The developer assumes no liability for damages resulting from its use.
 *
 * See the full license at: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */
#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

class ConfigManager
{
public:
    // JSON document for sensor configuration
    static DynamicJsonDocument sensorConfigData;

    ConfigManager(WiFiClientSecure &client);
    bool shouldFetchConfig();
    void fetchConfig();
    void parseConfig(const String &jsonPayload);
    bool loadSensorConfig();
    bool saveSensorConfig(String jsonString);
    bool fetchSensorConfig();
    void distributeSensorsToManagers(void);

private:
    WiFiClientSecure &client;
    const char *server = "soilmonitoring.smart-farming.org";
    const char *configUrl = "/sensors.json";
    const int httpsPort = 443;
};

#endif
