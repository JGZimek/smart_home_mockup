#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "esp_log.h"

// Konfiguracja LED
#define LED_PIN 14
#define NUM_LEDS 6  // Liczba diod

/*
 * @brief Inicjalizuje sterowanie diodami LED.
 */
bool init_led_control();

/*
 * @brief Ustawia kolor konkretnej diody.
 * @param ledIndex Indeks diody (0-5)
 * @param red Składowa czerwona (0-255)
 * @param green Składowa zielona (0-255)
 * @param blue Składowa niebieska (0-255)
 */
void set_led_color(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue);

/*
 * @brief Obsługuje diody – zmienia kolory co 4 sekundy.
 */
void handle_led_control();
