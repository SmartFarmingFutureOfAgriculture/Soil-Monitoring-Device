
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

#include "SleepManager.h"
#include <config.h>

SleepManager::SleepManager() {}

void SleepManager::enterDeepSleep() {
    Serial.println("😴 Gehe in Deep-Sleep-Modus...");
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
    esp_deep_sleep_start();
}
