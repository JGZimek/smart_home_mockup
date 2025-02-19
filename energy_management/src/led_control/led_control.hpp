#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "esp_log.h"

// LED Configuration
#define LED_PIN 14
#define NUM_LEDS 1

/*
 * @brief Initializes the LED control.
 */
bool init_led_control();

/*
 * @brief Sets the LED color.
 * @param red Red component (0-255)
 * @param green Green component (0-255)
 * @param blue Blue component (0-255)
 */
void set_led_color(uint8_t red, uint8_t green, uint8_t blue);

/*
 * @brief Handles the LED color cycling.
 */
void handle_led_control();
