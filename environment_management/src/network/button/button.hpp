#pragma once

#include <Arduino.h>
#include <functional>
#include "../access_point/access_point.hpp"
#include "esp_log.h"

/**
 * @brief Button class for detecting long-press events.
 *
 * This class encapsulates the functionality of a physical button,
 * including debouncing and long-press detection. It provides a begin() method
 * to initialize the button and a handle() method that must be called periodically.
 * A long-press callback is triggered when the button is held for a specified duration.
 */
class Button
{
public:
    // Default configuration constants
    static constexpr uint8_t DEFAULT_PIN = 13;             ///< Default GPIO pin for the button.
    static constexpr uint32_t DEFAULT_HOLD_TIME = 2000;    ///< Default required hold time in ms.
    static constexpr uint32_t DEFAULT_DEBOUNCE_DELAY = 50; ///< Default debounce delay in ms.

    // Logging tag for the button module.
    static const char *BUTTON_TAG;

    /**
     * @brief Construct a new Button object.
     *
     * @param pin The GPIO pin where the button is connected. Defaults to DEFAULT_PIN.
     * @param holdTime The time (in ms) the button must be held to trigger a long press.
     *                 Defaults to DEFAULT_HOLD_TIME.
     * @param debounceDelay The debounce delay (in ms) for filtering out noise.
     *                      Defaults to DEFAULT_DEBOUNCE_DELAY.
     */
    Button(uint8_t pin = DEFAULT_PIN, uint32_t holdTime = DEFAULT_HOLD_TIME, uint32_t debounceDelay = DEFAULT_DEBOUNCE_DELAY);

    /**
     * @brief Initializes the button.
     *
     * Configures the hardware pin and initializes internal state.
     *
     * @return true if initialization was successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Updates the button state.
     *
     * This method must be called periodically (e.g., in a task loop) to update
     * the button state and detect long-press events.
     */
    void handle();

    /**
     * @brief Sets the callback to be executed upon a long press.
     *
     * @param callback A callable with no parameters to be executed when a long press is detected.
     */
    void setLongPressCallback(std::function<void()> callback);

private:
    uint8_t _pin;                             ///< Button pin.
    uint32_t _holdTime;                       ///< Required hold time (ms) to trigger a long press.
    uint32_t _debounceDelay;                  ///< Debounce delay (ms).
    bool _currentState;                       ///< Current stable state of the button.
    bool _previousState;                      ///< Previous reading.
    unsigned long _lastDebounceTime;          ///< Timestamp when the input last changed.
    unsigned long _pressStartTime;            ///< Timestamp when the button was first pressed.
    bool _longPressTriggered;                 ///< Flag indicating if long-press event has been triggered.
    std::function<void()> _longPressCallback; ///< Callback for long press.
};
