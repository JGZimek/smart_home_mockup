#include <Arduino.h>

// put function declarations here:
void myFunction();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // Inicjalizacja komunikacji szeregowej
}

void loop() {
  // put your main code here, to run repeatedly:
  myFunction();
  delay(1000);  // Opóźnienie 1 sekundy
}

// put function definitions here:
void myFunction() {
  Serial.println("Hello world");
}
