#include "DHT.h"            //include DHT library to access all the functions

#define DHT_PIN 2           //define constants and constant variables
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);        //create an DHT class object with sensor type DHT11 and is connected to pin Digital 2 on the board  

void setup() {
  Serial.begin(9600);       //set serial communication at 9600 baud 

  Serial.println("Embedded Environmental Data Logger");  //print startup message
  Serial.println("V3: DHT11 Sensor Reading Started");

  dht.begin();   //initialize DHT11 sensor
}

void loop() {
  float humidity = dht.readHumidity();  //read humidity
  float temperatureC = dht.readTemperature();  //read temperature

  if (isnan(humidity) || isnan(temperatureC)) {       //check if both inputs are numbers
    Serial.println("Error: Failed to read from DHT11 sensor");    //output error message
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