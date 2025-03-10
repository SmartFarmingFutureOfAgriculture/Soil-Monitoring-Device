
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
#include "DataSender.h"
#include "config.h"

DataSender::DataSender(WiFiClientSecure &client) : client(client) {}

void DataSender::sendSensorData(const String &jsonPayload)
{
    // Connect to webserver
    Serial.println("📡 Sende HTTP-POST Anfrage...");
    if (!client.connect(server, httpsPort))
    {
        Serial.println("❌ Verbindung fehlgeschlagen!");
        return;
    }

    // Send data to webserver
    String fullUrl = String(dataUrl) + "?api_key=" + apiKey;
    client.print(String("POST ") + fullUrl + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + String(jsonPayload.length()) + "\r\n" +
                 "Connection: close\r\n\r\n" +
                 jsonPayload);

    // receive answer from webserver
    Serial.println("📩 Antwort:");
    while (client.connected() || client.available())
    {
        String line = client.readStringUntil('\n');
        Serial.println(line);
    }

    Serial.println("\n🔌 Verbindung schließen...");
    client.stop();
}
