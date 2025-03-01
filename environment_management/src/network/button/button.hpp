#pragma once

#include <Arduino.h>
#include <functional>
#include "../access_point/access_point.hpp"
#include "esp_log.h"

/**
 * @brief Class for detecting long-press events on a physical button.
 *
 * This class encapsulates the functionality of a physical button, including debouncing
 * and long-press detection. Call begin() once to initialize and then periodically call
 * handle() (e.g., in a FreeRTOS task) to update the state. A long-press callback is triggered
 * if the button is held longer than the specified hold time.
 */
class Button
{
public:
    /// Default configuration constants
    static constexpr uint8_t DEFAULT_PIN = 13;             ///< Default GPIO pin for the button.
    static constexpr uint32_t DEFAULT_HOLD_TIME = 2000;    ///< Required hold time (ms) to trigger a long press.
    static constexpr uint32_t DEFAULT_DEBOUNCE_DELAY = 50; ///< Debounce delay (ms).

    /// Logging tag for the Button module.
    static const char *BUTTON_TAG;

    /**
     * @brief Constructs a new Button object.
     *
     * @param pin GPIO pin to which the button is connected. Defaults to DEFAULT_PIN.
     * @param holdTime Time (in ms) the button must be held to trigger a long press.
     *                 Defaults to DEFAULT_HOLD_TIME.
     * @param debounceDelay Debounce delay (in ms) for filtering out noise. Defaults to DEFAULT_DEBOUNCE_DELAY.
     */
    Button(uint8_t pin = DEFAULT_PIN, uint32_t holdTime = DEFAULT_HOLD_TIME, uint32_t debounceDelay = DEFAULT_DEBOUNCE_DELAY);

    /**
     * @brief Initializes the button.
     *
     * Configures the hardware pin and initializes internal state.
     * @return true if initialization was successful.
     */
    bool begin();

    /**
     * @brief Updates the button state.
     *
     * This method should be called periodically (e.g., in a task loop) to update
     * the button state and detect long-press events.
     */
    void handle();

    /**
     * @brief Sets the callback function to be executed upon a long press.
     *
     * @param callback A callable object (e.g., lambda) with no parameters.
     */
    void setLongPressCallback(std::function<void()> callback);

private:
    uint8_t _pin;                             ///< GPIO pin where the button is connected.
    uint32_t _holdTime;                       ///< Required time (ms) to trigger a long press.
    uint32_t _debounceDelay;                  ///< Debounce delay (ms).
    bool _currentState;                       ///< Current stable state of the button.
    bool _previousState;                      ///< Previous reading of the button.
    unsigned long _lastDebounceTime;          ///< Timestamp when the last state change occurred.
    unsigned long _pressStartTime;            ///< Timestamp when the button was first pressed.
    bool _longPressTriggered;                 ///< Indicates if the long-press event has already been triggered.
    std::function<void()> _longPressCallback; ///< Callback to execute on long press.
};
