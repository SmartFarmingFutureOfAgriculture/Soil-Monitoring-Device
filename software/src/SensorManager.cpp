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

#include <ArduinoJson.h> 
#include "SensorManager.h"
#include <config.h>

SensorManager::SensorManager() {}

String SensorManager::getSensorDataJson() {
    DynamicJsonDocument doc(256);  // Anstatt StaticJsonDocument

    doc["sensor_id"] = "Node_1";
    doc["timestamp"] = "2024-02-23 12:00:00";

    JsonObject npk = doc["npk"].to<JsonObject>();  // Neuer Weg statt createNestedObject
    npk["n"] = 12;
    npk["p"] = 8;
    npk["k"] = 10;

    doc["ph"] = 6.8;
    doc["temperature"] = 22.5;
    doc["humidity"] = 50;
    doc["ec"] = 1100;
    doc["pressure"] = 1012.3;
    doc["capacitor_voltage"] = 3.2;
    doc["capacitor_charge"] = 62;

    String json;
    serializeJson(doc, json);
    return json;
}
