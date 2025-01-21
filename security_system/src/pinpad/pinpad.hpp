#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include "esp_log.h"

// Define rows and columns for the keypad
#define ROW_NUM 4    // Four rows
#define COLUMN_NUM 4 // Four columns

#define LED_PIN 2

// Declare row and column pins as extern
extern byte ROW_PINS[ROW_NUM];    // GPIO pins connected to row pins
extern byte COL_PINS[COLUMN_NUM]; // GPIO pins connected to column pins

// Define the keymap for the keypad
const char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Initialize the keypad object
extern Keypad pinpad;

/**
 * @brief Initializes the pinpad.
 *
 * Sets up the keypad object to use the defined rows and columns for input.
 */
bool init_pinpad();

/**
 * @brief Periodically handles pinpad input.
 *
 * Reads the key presses and handles the logic of entering a PIN, starting with '*' and ending with '#'.
 */
void handle_pinpad();

/**
 * @brief Resets the current PIN entry.
 *
 * Clears the buffer and resets the PIN entry process.
 */
void reset_pinpad_entry();

void light_up_LED_PIN();