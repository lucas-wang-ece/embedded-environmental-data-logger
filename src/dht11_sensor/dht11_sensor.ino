#include "DHT.h"            //include DHT library

#define DHT_PIN 2           //define constants and constant variables
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);        //create an dht class object with sensor type DHT11 and is connected to Digital 2 onn the board  

void setup() {
  Serial.begin(9600);

  Serial.println("Embedded Environmental Data Logger");  //print startup message
  Serial.println("V3: DHT11 Sensor Reading Started");

  dht.begin();   //initialize DHT11 sensor
}

void loop() {
  float humidity = dht.readHumidity();  //read humidity
  float temperatureC = dht.readTemperature();  //read temperature

  if (isnan(humidity) || isnan(temperatureC)) {       //check if any of the input is missing
    Serial.println("Error: Failed to read from DHT11 sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");   //print temperature
  Serial.print(temperatureC);
  Serial.print(" C");

  Serial.print(" | Humidity: ");  //print humidity
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}