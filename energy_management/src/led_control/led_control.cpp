#include "led_control.hpp"

#define LED_CONTROL_TAG "led_control"

// LED Array
CRGB led[NUM_LEDS];

/*
 * @brief Initializes the LED system.
 */
bool init_led_control()
{
    ESP_LOGI(LED_CONTROL_TAG, "Initializing LED control...");
    FastLED.addLeds<WS2812, LED_PIN, RGB>(led, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(100); // Set LED brightness
    return true;
}

/*
 * @brief Sets the LED to a specific color.
 */
void set_led_color(uint8_t red, uint8_t green, uint8_t blue)
{
    led[0] = CRGB(red, green, blue);
    FastLED.show();
    ESP_LOGI(LED_CONTROL_TAG, "LED Color -> R: %d, G: %d, B: %d", red, green, blue);
}

/*
 * @brief Handles LED color.
 */
void handle_led_control()
{
    int red=0;
    int green=0;
    int blue=255;

    set_led_color(red, green, blue);
}