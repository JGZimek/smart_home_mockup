#include "pinpad.hpp"
#include "../mqtt/mqtt.hpp" // Korzystamy z funkcji do wysyłania kodu po MQTT
#include "esp_log.h"

#define PINPAD_TAG "app_pinpad"

// Define the actual row and column pins here (the definitions)
byte ROW_PINS[ROW_NUM] = {13, 12, 14, 27};    // GPIO pins connected to row pins
byte COL_PINS[COLUMN_NUM] = {26, 25, 33, 32}; // GPIO pins connected to column pins

// Initialize the keypad object with row pins, column pins, and keymap
Keypad pinpad = Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROW_NUM, COLUMN_NUM);

static String current_pin = "";   // Stores the entered PIN
static bool entering_pin = false; // Indicates if we're in the process of entering a PIN

bool init_pinpad()
{
    ESP_LOGI(PINPAD_TAG, "Pinpad initialized.");
    return true;
}

void handle_pinpad()
{
    char key = pinpad.getKey(); // Read the key pressed
    if (key)
    {
        ESP_LOGI(PINPAD_TAG, "Key pressed: %c", key);

        // Start entering the PIN when '*' is pressed
        if (key == '*')
        {
            entering_pin = true;
            current_pin = ""; // Reset the current PIN buffer
            ESP_LOGI(PINPAD_TAG, "PIN entry started.");
        }
        // End of PIN entry when '#' is pressed
        else if (key == '#')
        {
            if (entering_pin && current_pin.length() > 0)
            {
                ESP_LOGI(PINPAD_TAG, "PIN entry complete: %s", current_pin.c_str());
                publish_pinpad_event(current_pin); // Wywołanie funkcji z modułu MQTT do wysłania kodu
            }
            reset_pinpad_entry(); // Zawsze resetuj po zakończeniu wpisywania PINu
        }
        // Add digit to the PIN if we're in the process of entering it
        else if (entering_pin && isdigit(key))
        {
            current_pin += key; // Add the digit to the current PIN
            ESP_LOGI(PINPAD_TAG, "Current PIN: %s", current_pin.c_str());
        }
        // Cancel/clear entry if 'D' is pressed
        else if (key == 'D')
        {
            ESP_LOGI(PINPAD_TAG, "PIN entry canceled.");
            reset_pinpad_entry();
        }
    }
}

void reset_pinpad_entry()
{
    current_pin = "";
    entering_pin = false;
    ESP_LOGI(PINPAD_TAG, "PIN entry reset.");
}
