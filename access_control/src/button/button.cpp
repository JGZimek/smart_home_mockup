// button.cpp
#include "button.hpp"

Button::Button(uint8_t pin, uint32_t longPressDuration)
    : pin(pin), longPressDuration(longPressDuration), longPressDetected(false), pressStartTime(0) {}

void Button::begin()
{
    pinMode(pin, INPUT_PULLUP);
    attachInterruptArg(digitalPinToInterrupt(pin), handleInterrupt, this, CHANGE);
}

bool Button::isLongPressed()
{
    if (longPressDetected)
    {
        longPressDetected = false; // Reset the flag after detection
        return true;
    }
    return false;
}

void IRAM_ATTR Button::handleInterrupt(void *arg)
{
    Button *button = static_cast<Button *>(arg);
    unsigned long currentTime = millis();

    if (digitalRead(button->pin) == LOW)
    {
        button->pressStartTime = currentTime;
    }
    else
    {
        if ((currentTime - button->pressStartTime) >= button->longPressDuration)
        {
            button->longPressDetected = true;
        }
    }
}
