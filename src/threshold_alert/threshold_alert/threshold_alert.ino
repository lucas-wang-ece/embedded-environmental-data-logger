#include "DHT.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11

#define ALERT_LED_PIN LED_BUILTIN
#define TEMPERATURE_THRESHOLD_C 25.0

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);

  Serial.println("Embedded Environmental Data Logger");
  Serial.println("V4: Threshold Alert Started");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  if (isnan(humidity) || isnan(temperatureC)) {
    Serial.println("Error: Failed to read from DHT11 sensor");
    digitalWrite(ALERT_LED_PIN, LOW);
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" C");

  Serial.print(" | Humidity: ");
  Serial.print(humidity);
  Serial.print(" %");

  if (temperatureC >= TEMPERATURE_THRESHOLD_C) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    Serial.println(" | Alert: ON");
  } else {
    digitalWrite(ALERT_LED_PIN, LOW);
    Serial.println(" | Alert: OFF");
  }

  delay(2000);
}