#define RELAY_PIN 5  // Connect IN1 of relay to GPIO 5

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Turn relay off initially (active LOW)
  digitalWrite(RELAY_PIN, HIGH);

  Serial.println("Starting Relay Test...");
}

void loop() {
  Serial.println("Relay ON");
  digitalWrite(RELAY_PIN, LOW);  // Turn ON
  delay(1000);

  Serial.println("Relay OFF");
  digitalWrite(RELAY_PIN, HIGH); // Turn OFF
  delay(1000);
}
