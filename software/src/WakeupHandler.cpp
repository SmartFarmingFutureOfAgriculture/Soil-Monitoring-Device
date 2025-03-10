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

#include <WakeupHandler.h>
#include <esp_sleep.h>
#include <config.h>

/**
 * @brief Retrieves the reason for the ESP32 wake-up.
 * @return A string describing the cause of wake-up.
 */
String WakeupHandler::getWakeupReason()
{
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    switch (cause)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        return "Button pressed";
    case ESP_SLEEP_WAKEUP_EXT1:
        return "LoRa signal received";
    case ESP_SLEEP_WAKEUP_TIMER:
        return "Timer triggered";
    case ESP_SLEEP_WAKEUP_WIFI: // (light sleep only)
        return "WiFi";
    case ESP_SLEEP_WAKEUP_UNDEFINED: // reset
        return "ESP_SLEEP_WAKEUP_UNDEFINED";
    default:
        return "Power-on";
    }
}
