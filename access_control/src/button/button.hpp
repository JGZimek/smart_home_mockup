// button.hpp
#pragma once

#include <Arduino.h>

class Button
{
public:
    Button(uint8_t pin, uint32_t longPressDuration = 3000);

    /**
     * @brief Initializes the button (sets up GPIO and ISR).
     */
    void begin();

    /**
     * @brief Checks if a long press has been detected.
     * @return true if a long press was detected, false otherwise.
     */
    bool isLongPressed();

private:
    uint8_t pin;
    uint32_t longPressDuration;
    volatile bool longPressDetected;
    volatile unsigned long pressStartTime;

    /**
     * @brief Interrupt Service Routine for button press.
     */
    static void IRAM_ATTR handleInterrupt(void *arg);
};
