#include "DHT.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  Serial.println("Embedded Environmental Data Logger");
  Serial.println("V3: DHT11 Sensor Reading Started");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  if (isnan(humidity) || isnan(temperatureC)) {
    Serial.println("Error: Failed to read from DHT11 sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" C");

  Serial.print(" | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}