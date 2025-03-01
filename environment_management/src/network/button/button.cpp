#include "button.hpp"

// Define the static BUTTON_TAG variable.
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
    // Domyślny callback:
    // - Jeśli tryb AP nie jest aktywny, uruchamia AP.
    // - Jeśli tryb AP już działa, wywołuje zakończenie trybu AP.
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
    bool reading = (digitalRead(_pin) == LOW);
    ESP_LOGV(BUTTON_TAG, "Button reading: %s", reading ? "PRESSED" : "RELEASED");

    if (reading != _previousState)
    {
        _lastDebounceTime = millis();
        ESP_LOGV(BUTTON_TAG, "Debounce reset");
    }
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
