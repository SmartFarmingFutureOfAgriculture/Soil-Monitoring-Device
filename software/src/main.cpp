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


#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
#include "SensorManager.h"
#include "ConfigManager.h"
#include "DataSender.h"
#include "SleepManager.h"
#include <config.h>

// Klasseninstanzen
WiFiManager wifiManager(ssid, password, client);
SensorManager sensorManager;
ConfigManager configManager(client);
DataSender dataSender(client);
SleepManager sleepManager;

void setup()
{
    // Reducing the frequency to save energy
    setCpuFrequencyMhz(80);

    Serial.begin(9600);
    delay(1000);

    // Load sensor configuration from storage
    if (!configManager.loadSensorConfig())
    {
        // ...if there is no data in the flash, get it from web server
        configManager.fetchSensorConfig();
    }

    // Static class variable "sensorConfigData" contains sensors' configuration
    configManager.distributeSensorsToManagers();

    // wifiManager.connect();

    // Determine the reason for waking up
    //if (configManager.shouldFetchConfig())
    //{
     //   configManager.fetchConfig();
    //}
    // Read sensor data
    String sensorDataJson = sensorManager.getSensorDataJson();
    // Send sensor data to webserver
    dataSender.sendSensorData(sensorDataJson);

    sleepManager.enterDeepSleep();
}

void loop() {}
