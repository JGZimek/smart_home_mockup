#pragma once

#include <Arduino.h>

/**
 * @brief Initializes the buzzer.
 *
 * Configures the GPIO pin used for controlling the buzzer.
 * @return true if initialization was successful, false otherwise.
 */
bool init_buzzer();

/**
 * @brief Handles the buzzer.
 *
 * This function should be called periodically to manage the buzzer's state.
 */
void handle_buzzer();

/**
 * @brief Plays a short beep with the buzzer.
 *
 * Generates a short beep sound, useful for simple notifications.
 *
 * @param duration_ms The duration of the beep in milliseconds.
 */
void buzzer_short_beep(int duration_ms);

/**
 * @brief Plays a long beep with the buzzer.
 *
 * Generates a long beep sound, useful for warning signals.
 *
 * @param duration_ms The duration of the beep in milliseconds.
 */
void buzzer_long_beep(int duration_ms);

/**
 * @brief Schedules a sequence of beeps.
 *
 * Generates a sequence of beeps, useful for specific events or alerts.
 * The sequence will be handled asynchronously in the FreeRTOS task.
 *
 * @param duration_ms The duration of each beep in milliseconds.
 * @param count The number of beeps to play in the sequence.
 */
void buzzer_beep_sequence(int duration_ms, int count);
