#include "DHT.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11

#define ALERT_LED_PIN LED_BUILTIN     // Use the built-in LED as the alert indicator.
#define TEMPERATURE_THRESHOLD_C 25.0   //define threshold temperature

DHT dht(DHT_PIN, DHT_TYPE);     // Create a DHT sensor object using the selected pin and sensor type.

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);  //select the alert LED as the output pin

  Serial.println("Embedded Environmental Data Logger");  //print startup message
  Serial.println("V4: Threshold Alert Started");

  dht.begin();   //initialize the DHT11 sensor
}

void loop() {
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  if (isnan(humidity) || isnan(temperatureC)) {           //check if both inputs are numbers
    Serial.println("Error: Failed to read from DHT11 sensor");    //print error message
    digitalWrite(ALERT_LED_PIN, LOW);       //set output LED_PIN as low voltage so it will not light up
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" C");

  Serial.print(" | Humidity: ");
  Serial.print(humidity);
  Serial.print(" %");

  if (temperatureC >= TEMPERATURE_THRESHOLD_C) {        //check if surrounding temperature is higher than threshold temperature
    digitalWrite(ALERT_LED_PIN, HIGH);
    Serial.println(" | Alert: ON");             //if yes, set alert to HIGH
  } else {
    digitalWrite(ALERT_LED_PIN, LOW);
    Serial.println(" | Alert: OFF");            //if no, set alert to OFF
  }

  delay(2000);
}