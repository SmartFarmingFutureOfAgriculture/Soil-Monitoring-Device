#pragma once
#include <Arduino.h>

/**
 * @class MonitoringNode
 * @brief Main class responsible for controlling the device's core functionality.
 */
class MonitoringNode
{
public:
    // Initializes the device and handles wake-up logic.
    void begin(float voltage, float current, float power);
    
private:
    // Generates hash of ESP's MAC address
    void generateTokenFromMac(const uint8_t *mac, char *outputToken, size_t outputSize);

    // Handles wake-up by button press (GPIO).
    void handleButtonPress();

    // Handles wake-up by LoRa signal.
    void handleLoRaSignal();

    // Handles wake-up by timer interrupt.
    void handleTimerWakeup(float, float, float);

    // Handles power-on initialization.
    void handlePowerOn();

    // Start deep sleep
    void goToDeepSleep(void);
};