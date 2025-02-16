#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

/* Tag dla logów ESP */
#define PANEL_CONTROL_TAG "panel_control"

// Definicje pinów serwomechanizmów
#define HORIZONTAL_SERVO_PIN  12
#define VERTICAL_SERVO_PIN    13

// Definicje limitów serwomechanizmów
#define SERVO_H_LIMIT_HIGH 180
#define SERVO_H_LIMIT_LOW  65
#define SERVO_V_LIMIT_HIGH 120
#define SERVO_V_LIMIT_LOW  15

// Definicje pinów fotorezystorów
#define LDR_TR_PIN  34  // LDR top right
#define LDR_TL_PIN  35  // LDR top left
#define LDR_BR_PIN  32  // LDR bottom right
#define LDR_BL_PIN  33  // LDR bottom left

/**
 * @brief Inicjalizuje serwomechanizmy i ustawia ich pozycję startową.
 */
void init_panel_control();

/**
 * @brief Odczytuje wartości z fotorezystorów.
 */
void read_LDRs();

/**
 * @brief Steruje serwomechanizmem poziomym w zależności od odczytów LDR.
 */
void control_horizontal_servo();

/**
 * @brief Steruje serwomechanizmem pionowym w zależności od odczytów LDR.
 */
void control_vertical_servo();

/**
 * @brief Obsługuje logikę śledzenia światła.
 */
void handle_panel_control();
