#include "panel_control.hpp"

// Tworzenie obiektów serwomechanizmów
static Servo horizontalServo;
static Servo verticalServo;

// Aktualne kąty serwomechanizmów
static int horizontalServoAngle = 90;
static int verticalServoAngle = 90;

// Zmienna do przechowywania wartości z fotorezystorów
static int tr = 0, tl = 0, br = 0, bl = 0;
static int tol = 50;  // Tolerancja różnic w odczytach LDR

void init_panel_control() {
    ESP_LOGI(PANEL_CONTROL_TAG, "Initializing panel control...");

    // Przypisanie serwomechanizmów do pinów
    horizontalServo.attach(HORIZONTAL_SERVO_PIN);
    verticalServo.attach(VERTICAL_SERVO_PIN);

    // Ustawienie początkowych pozycji serwomechanizmów
    horizontalServo.write(horizontalServoAngle);
    verticalServo.write(verticalServoAngle);

    ESP_LOGI(PANEL_CONTROL_TAG, "Panel control initialized at default positions.");
}

void read_LDRs() {
    // Odczyt wartości z fotorezystorów
    tr = analogRead(LDR_TR_PIN);  // top right
    tl = analogRead(LDR_TL_PIN);  // top left
    br = analogRead(LDR_BR_PIN);  // bottom right
    bl = analogRead(LDR_BL_PIN);  // bottom left

    ESP_LOGI(PANEL_CONTROL_TAG, "LDR Readings - TR: %d, TL: %d, BR: %d, BL: %d", tr, tl, br, bl);
}

void control_horizontal_servo() {
    // Średnie wartości LDR z lewej i prawej strony
    int avl = (tl + bl) / 2;
    int avr = (tr + br) / 2;

    // Różnica między stronami
    int dhoriz = avl - avr;

    // Sprawdzenie tolerancji i sterowanie serwem poziomym
    if (abs(dhoriz) > tol) {
        if (avl > avr) {
            horizontalServoAngle = max(horizontalServoAngle - 1, SERVO_H_LIMIT_LOW);
        } else if (avr > avl) {
            horizontalServoAngle = min(horizontalServoAngle + 1, SERVO_H_LIMIT_HIGH);
        }
        horizontalServo.write(horizontalServoAngle);
        ESP_LOGI(PANEL_CONTROL_TAG, "Horizontal Servo Angle: %d", horizontalServoAngle);
    }
}

void control_vertical_servo() {
    // Średnie wartości LDR z góry i dołu
    int avt = (tl + tr) / 2;
    int avd = (bl + br) / 2;

    // Różnica między góra a dołem
    int dvert = avt - avd;

    // Sprawdzenie tolerancji i sterowanie serwem pionowym
    if (abs(dvert) > tol) {
        if (avt > avd) {
            verticalServoAngle = min(verticalServoAngle + 1, SERVO_V_LIMIT_HIGH);
        } else if (avd > avt) {
            verticalServoAngle = max(verticalServoAngle - 1, SERVO_V_LIMIT_LOW);
        }
        verticalServo.write(verticalServoAngle);
        ESP_LOGI(PANEL_CONTROL_TAG, "Vertical Servo Angle: %d", verticalServoAngle);
    }
}

void handle_panel_control() {
    read_LDRs();
    control_horizontal_servo();
    control_vertical_servo();
}
