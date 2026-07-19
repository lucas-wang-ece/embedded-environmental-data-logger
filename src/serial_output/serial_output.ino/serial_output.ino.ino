void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Embedded Environmental Data Logger");
  Serial.println("V2: Serial Monitor Output Started");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("System running... LED ON");
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("System running... LED OFF");
  delay(1000);
}