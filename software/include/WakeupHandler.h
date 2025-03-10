#pragma once
#include <Arduino.h>

/**
 * @class WakeupHandler
 * @brief Determines the reason for the device waking up from sleep.
 */
class WakeupHandler {
public:
    /**
     * @brief Retrieves the wake-up reason.
     * @return A string describing the wake-up reason.
     */
    static String getWakeupReason();
};
