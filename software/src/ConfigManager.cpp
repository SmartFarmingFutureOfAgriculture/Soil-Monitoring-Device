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
#include "ConfigManager.h"
#include "esp_sleep.h"
#include <ArduinoJson.h>
// NVS storage for JASON data
#include <Preferences.h>
#include <config.h>

ConfigManager::ConfigManager(WiFiClientSecure &client) : client(client) {}

// JSON document for sensor configuration
DynamicJsonDocument sensorConfigData(2048);

// JSON data storage in Flash
Preferences preferences;

/**
 * @brief
 * Dectects the wakeup reason
 *
 * @return true (if wakeup was caused by reset or power up)
 * @return false (every other case)
 */
bool ConfigManager::shouldFetchConfig()
{
    uint64_t wakeup_reason = esp_sleep_get_wakeup_cause();
    return (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED);
}



/**
 * @brief Loading JSON data from Flash
 *
 * "sensorConfigData" holds the sensors' configuration data
 * @return true, if data is stored in the flash and serialization works
 * @return false, if no data is stored in the flash or serialization fails
 */
bool ConfigManager::loadSensorConfig()
{
    // Load the saved configuration named "sensor_config" and save it in "savedConfigRaw"
    preferences.begin("sensor_config", true);
    String savedConfigRaw = preferences.getString("config", "");
    preferences.end();

    // If there is no data in the flash
    if (savedConfigRaw == "")
        return false;

    // Deserialize JSON data for easy access to individual elements (saved in "sensorConfigData")
    DeserializationError error = deserializeJson(sensorConfigData, savedConfigRaw);
    if (error)
    {
        Serial.println("Error: " + (String)error.c_str());
        return false;
    }
    return true;
}
// end of function ----------------


/**
 * @brief Request JSON configuration file from a web server with PHP available
 *
 * - Sends an HTTP GET request to the address in configUrl on the server.
 * - Server processes the request and sends back the requested resource (JSON file)
 * - The response can then be read in and further processed by the client
 */
void ConfigManager::fetchConfig()
{
    // Provide SSL server certificate
    client.setCACert(root_ca);

    // Connect to the HTTP server
    if (!client.connect(server, httpsPort))
    {
        Serial.println("❌ Verbindung fehlgeschlagen!");
        return;
    }

    // Retrieve (GET) the contents from the file (from variable: configURL)
    client.print(String("GET ") + configUrl + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    String jsonPayload;
    while (client.connected() || client.available())
    {
        String line = client.readStringUntil('\n');
        jsonPayload += line;
    }
    client.stop();

    // Stores the JSON data
    if (!saveSensorConfig(jsonPayload))
    {
        Serial.println("Error, can't save data in flash memory");
    }

    //  parseConfig(jsonPayload);
}
// end of function ----------------


/**
 * @brief Stores the JSON data from the web server in the ESP32's flash memory
 *
 * @param String jsonString
 *
 */
bool ConfigManager::saveSensorConfig(String jsonString)
{
    if (preferences.begin("sensor_config", false))
    {
        preferences.putString("config", jsonString);
        preferences.end();
        return true;
    }
    return false;
}
// end of function ----------------


/**
 * @brief 
 * 
 */
void distributeSensorsToManagers(void) {
    JsonArray sensors = sensorConfigData["shared"]["sensor_config"]["sensors"];

    // loop goes through each element from the sensors list and stores it in the variable sensor.
    for (JsonObject sensor : sensors) {
        String protocol = sensor["protocol"].as<String>();

        if (protocol == "I2C") {
            Serial.println("I2C-Sensor erkannt: " + sensor["name"].as<String>());
            I2CManager::registerSensor(sensor);
        }
        else if (protocol == "Modbus") {
            Serial.println("Modbus-Sensor erkannt: " + sensor["name"].as<String>());
            //ModbusManager::registerSensor(sensor);
        }
        else {
            // Insert another interface here
            Serial.println("Unbekanntes Protokoll für Sensor: " + sensor["name"].as<String>());
        }
    }
}


//*************************** */
void ConfigManager::parseConfig(const String &jsonPayload)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error)
    {
        Serial.print("❌ JSON-Fehler: ");
        Serial.println(error.f_str());
        return;
    }

    Serial.println("🌍 Gateway-Infos:");
    Serial.print("Name: ");
    Serial.println(doc["gateway"]["name"].as<String>());
    Serial.print("Ort: ");
    Serial.println(doc["gateway"]["title"].as<String>());
    Serial.print("Land: ");
    Serial.println(doc["gateway"]["country"].as<String>());

    Serial.println("\n🔎 Sensoren:");
    JsonArray sensors = doc["sensors"].as<JsonArray>();

    for (JsonVariant sensor : sensors)
    {
        Serial.println("--------------------");
        Serial.print("📡 Name: ");
        Serial.println(sensor["name"].as<String>());
        Serial.print("📝 Titel: ");
        Serial.println(sensor["title"].as<String>());
        Serial.print("⚡ Status: ");
        Serial.println(sensor["status"].as<String>());
        Serial.print("🔌 Schnittstelle: ");
        Serial.println(sensor["protocol"].as<String>());

        if (sensor["protocol"] == "I2C")
        {
            Serial.print("  ➝ I2C-Adresse: ");
            Serial.println(sensor["protocol_config"]["address"].as<String>());
            Serial.print("  ➝ I2C-Speed: ");
            Serial.println(sensor["protocol_config"]["speed"].as<String>());
        }
        else if (sensor["protocol"] == "Modbus")
        {
            Serial.print("  ➝ Modbus-Adresse: ");
            Serial.println(sensor["protocol_config"]["address"].as<int>());
            Serial.print("  ➝ Baudrate: ");
            Serial.println(sensor["protocol_config"]["baud_rate"].as<int>());
        }

        if (sensor["function"]["Plant_type"].is<String>())
        {
            Serial.print("🌱 Pflanzentyp: ");
            Serial.println(sensor["function"]["Plant_type"].as<String>());
            Serial.print("🪴 Wachstumsphase: ");
            Serial.println(sensor["function"]["Growth_stage"].as<String>());
            Serial.print("📅 Pflanzdatum: ");
            Serial.println(sensor["function"]["Planting_date"].as<String>());
        }
    }

    Serial.println("\n✅ Sensor-Konfiguration abgeschlossen!");
}

//************************************************************* */
// Anfrage an ThingsBoard senden und Antwort empfangen
//**************************************************************/
bool ConfigManager::fetchSensorConfig()
{
#ifdef DEBUG
    Debug::drawWrappedText(u8g2, "Sende Anfrage an ThingsBoard...", 0, 12, 128, 12);
    delay(2500);
#endif

    client.setCACert(root_ca);
    if (!client.connect(server, httpsPort))
    {
#ifdef DEBUG
        Debug::drawWrappedText(u8g2, "Verbindung fehlgeschlagen!", 0, 12, 128, 12);
        delay(2500);
#endif
        return false;
    }

    client.print(String("GET /api/v1/") + token + "/attributes HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    // Header empfangen
    while (client.connected())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
#ifdef DEBUG
            Debug::drawWrappedText(u8g2, "Headers empfangen!", 0, 12, 128, 12);
            delay(2500);
#endif
            break;
        }
    }

    // Antwort von ThingsBoard auslesen
    String response;
    while (client.available())
    {
        response += client.readString();
    }

    // Empfangene Daten von ThingsBoard ausgeben
#ifdef DEBUG
    Debug::drawWrappedText(u8g2, "Empfangene JSON-Daten:", 0, 12, 128, 12);
    delay(2500);
    Debug::drawWrappedText(u8g2, response.c_str(), 0, 12, 128, 12);
    delay(2500);
#endif

    // Keine Daten empfangen, Fehler ausgeben
    if (response.length() == 0)
    {
#ifdef DEBUG
        Debug::drawWrappedText(u8g2, "Fehler: Keine Daten empfangen!", 0, 12, 128, 12);
        delay(2500);
#endif
        return false;
    }

    // 🛠 JSON verarbeiten
    DeserializationError error = deserializeJson(sensorConfigData, response);
    if (error)
    {
#ifdef DEBUG
        Debug::drawWrappedText(u8g2, "Fehler beim JSON-Parsing: ", 0, 12, 128, 12);
        delay(2500);
        Debug::drawWrappedText(u8g2, error.c_str(), 0, 12, 128, 12);
        delay(2500);
#endif
        return false;
    }

    // Konfiguration speichern
    saveSensorConfig(response);

    return true;
}