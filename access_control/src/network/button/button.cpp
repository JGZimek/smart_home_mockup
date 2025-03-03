#include "button.hpp"

// Define the static logging tag.
const char *Button::BUTTON_TAG = "app_button";

Button::Button(uint8_t pin, uint32_t holdTime, uint32_t debounceDelay)
    : _pin(pin),
      _holdTime(holdTime),
      _debounceDelay(debounceDelay),
      _currentState(HIGH),
      _previousState(HIGH),
      _lastDebounceTime(0),
      _pressStartTime(0),
      _longPressTriggered(false)
{
    // Default long-press callback:
    // If the AP mode is inactive, start AP mode;
    // if AP mode is active, request exit from AP mode.
    _longPressCallback = []()
    {
        if (AccessPoint::isAPActive())
        {
            ESP_LOGI(Button::BUTTON_TAG, "Button long-pressed. Exiting AP mode...");
            AccessPoint::requestAPExit();
        }
        else
        {
            ESP_LOGI(Button::BUTTON_TAG, "Button long-pressed. Launching AP mode for reconfiguration...");
            AccessPoint::startAPTask("ESP32_Config", "config123");
        }
    };
}

bool Button::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    _currentState = digitalRead(_pin);
    _previousState = _currentState;
    _lastDebounceTime = millis();
    return true;
}

void Button::setLongPressCallback(std::function<void()> callback)
{
    _longPressCallback = callback;
}

void Button::handle()
{
    // Read the current state (active low)
    bool reading = (digitalRead(_pin) == LOW);
    ESP_LOGV(BUTTON_TAG, "Button reading: %s", reading ? "PRESSED" : "RELEASED");

    // If the reading has changed, reset the debounce timer.
    if (reading != _previousState)
    {
        _lastDebounceTime = millis();
        ESP_LOGV(BUTTON_TAG, "Debounce reset");
    }

    // If enough time has passed since the last change, update the stable state.
    if ((millis() - _lastDebounceTime) > _debounceDelay)
    {
        if (reading != _currentState)
        {
            _currentState = reading;
            if (_currentState)
            {
                _pressStartTime = millis();
                _longPressTriggered = false;
                ESP_LOGV(BUTTON_TAG, "Button pressed, timer started");
            }
        }
    }
    _previousState = reading;

    // If the button is pressed and a long press hasn't yet been triggered, check elapsed time.
    if (_currentState && !_longPressTriggered)
    {
        unsigned long elapsed = millis() - _pressStartTime;
        ESP_LOGV(BUTTON_TAG, "Button held for %lu ms", elapsed);
        if (elapsed >= _holdTime)
        {
            _longPressTriggered = true;
            ESP_LOGD(BUTTON_TAG, "Long press detected!");
            if (_longPressCallback)
            {
                _longPressCallback();
            }
        }
    }
}
