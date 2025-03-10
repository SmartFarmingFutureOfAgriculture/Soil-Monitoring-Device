#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <Arduino.h>
#include "esp_sleep.h"

class SleepManager {
public:
    SleepManager();
    void enterDeepSleep();
private:
    const uint64_t SLEEP_DURATION = 15 * 60 * 1000000ULL;  // 15 Minuten in Mikrosekunden
};

#endif
