#include "led_control.hpp"

#define LED_CONTROL_TAG "led_control"

// Tablica diod
CRGB leds[NUM_LEDS];

/*
 * @brief Inicjalizuje diody LED.
 */
bool init_led_control()
{
    ESP_LOGI(LED_CONTROL_TAG, "Initializing LED control...");
    FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(100);
    return true;
}

/*
 * @brief Ustawia kolor konkretnej diody.
 */
void set_led_color(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue)
{
    if (ledIndex < NUM_LEDS) {
        leds[ledIndex] = CRGB(red, green, blue);
        FastLED.show();
        ESP_LOGI(LED_CONTROL_TAG, "LED %d -> R: %d, G: %d, B: %d", ledIndex, red, green, blue);
    } else {
        ESP_LOGW(LED_CONTROL_TAG, "Invalid LED index: %d", ledIndex);
    }
}

/*
 * @brief Obsługa diod – zmienia kolory co 4 sekundy.
 */
void handle_led_control()
{
    // Pierwszy zestaw kolorów
    set_led_color(0, 255, 0, 0);  // Dioda 1 - czerwony
    set_led_color(1, 0, 255, 0);  // Dioda 2 - zielony
    set_led_color(2, 0, 0, 255);  // Dioda 3 - niebieski
    set_led_color(3, 255, 255, 0); // Dioda 4 - żółty
    set_led_color(4, 255, 0, 255); // Dioda 5 - różowy
    set_led_color(5, 0, 255, 255); // Dioda 6 - turkusowy

    ESP_LOGI(LED_CONTROL_TAG, "Handling LED control - Set 1");
    delay(4000);  // Odczekaj 4 sekundy

    // Drugi zestaw kolorów
    set_led_color(0, 255, 165, 0);  // Dioda 1 - pomarańczowy
    set_led_color(1, 75, 0, 130);   // Dioda 2 - fioletowy
    set_led_color(2, 0, 255, 127);  // Dioda 3 - morski
    set_led_color(3, 128, 0, 128);  // Dioda 4 - purpurowy
    set_led_color(4, 173, 255, 47); // Dioda 5 - oliwkowy
    set_led_color(5, 255, 20, 147); // Dioda 6 - róż głęboki

    ESP_LOGI(LED_CONTROL_TAG, "Handling LED control - Set 2");
    delay(4000);  // Odczekaj 4 sekundy
}
