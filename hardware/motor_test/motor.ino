#include <Wire.h>

#define PIN 4

void setup() {
  pinMode(PIN, OUTPUT);  // Set pin 5 as output
}

void loop() {
  digitalWrite(PIN, HIGH);  // Turn ON
  delay(3000);              // Wait 3 seconds

  digitalWrite(PIN, LOW);   // Turn OFF
  delay(3000);              // Wait 3 seconds
}