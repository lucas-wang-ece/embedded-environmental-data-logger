#include "DHT.h"
#include <SPI.h>
#include <SD.h>


#define DHT_PIN 2
#define DHT_TYPE DHT11
#define SD_CS_PIN 10        // SD card chip select pin
#define ALERT_LED_PIN LED_BUILTIN
#define TEMPERATURE_THRESHOLD_C 25.0

DHT dht(DHT_PIN, DHT_TYPE);

const char LOG_FILE_NAME[] = "sensor7.csv";      // CSV file name


volatile bool sampleRequested = false;    // This flag is set by the timer interrupt and checked in loop().


volatile unsigned int timerTickCount = 0;   //increments every one second; a new sample will be requested when it reaches 2, so one sample is requested every two seconds

void setupTimer1() {
  noInterrupts();     // Disable all other interrupts while configuring Timer1.

  TCCR1A = 0;     // Clear Timer1 control registers.
  TCCR1B = 0;

  TCNT1 = 0;     // Reset Timer1 counter.

  /*
    Configure Timer1 for 1 Hz interrupt.   16bits

    Arduino UNO clock speed: 16 MHz
    Prescaler: 1024

    Timer frequency = 16,000,000 / 1024 = 15625 Hz
    Selecting other prescalers will still make the timer frequency too large

    To trigger once per second:
    OCR1A = 15625 - 1 = 15624
  */
  OCR1A = 15624;

  TCCR1B |= (1 << WGM12);   // Enable Clear Timer on Compare Match mode.

  TCCR1B |= (1 << CS12) | (1 << CS10);    // Set prescaler to 1024.

  TIMSK1 |= (1 << OCIE1A);    // Enable Timer1 compare match interrupt.

  interrupts();     // Re-enable interrupts.
}

// Timer1 interrupt service routine.
// This runs automatically once per second.
ISR(TIMER1_COMPA_vect) {
  timerTickCount++;

  // Request a sensor sample every 2 seconds.
  if (timerTickCount >= 2) {
    timerTickCount = 0;
    sampleRequested = true;
  }
}

void writeCsvHeaderIfNeeded() {
  if (!SD.exists(LOG_FILE_NAME)) {
    File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);      // Only write the CSV header if the file does not already exist

    if (logFile) {
      logFile.println("time_ms,temperature_c,humidity_percent,alert_status,checksum");       //print head message in the file, telling readers what each column represent
      logFile.close();      //close the file in order to store the data in the SD card
      Serial.println("CSV header written.");
    } else {
      Serial.println("Error: Could not create sensor7.csv");     //print error message
    }
  }
}

byte calculateChecksum(String dataLine) {
  byte checksum = 0;

  for (unsigned int i = 0; i < dataLine.length(); i++) {
    checksum += dataLine[i];
  }

  return checksum;
}

void logSensorData(unsigned long timeMs, float temperatureC, float humidity, bool alertOn) {     //access all the data
  String dataLine = "";

  dataLine += String(timeMs);
  dataLine += ",";
  dataLine += String(temperatureC);
  dataLine += ",";
  dataLine += String(humidity);
  dataLine += ",";
  dataLine += alertOn ? "ON" : "OFF";

  byte checksum = calculateChecksum(dataLine);

  File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

  if (logFile) {
    logFile.print(dataLine);
    logFile.print(",");
    logFile.println(checksum);

    logFile.close();

    Serial.print("SD log: saved | Checksum: ");
    Serial.println(checksum);
  } else {
    Serial.println("Error: Could not open sensor7.csv");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);         //select the alert LED as the output pin

  Serial.println("Embedded Environmental Data Logger");           //print startup message
  Serial.println("V7: Checksum-Based SD Card Logging Started");

  dht.begin();      //initialize the DHT11 sensor

  Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {             //check if SD card is initialized successfully
    Serial.println("Error: SD card initialization failed.");
    Serial.println("Check wiring, SD card insertion, and FAT32 formatting.");
    return;
  }

  Serial.println("SD card initialized successfully.");

  writeCsvHeaderIfNeeded();

  setupTimer1();      // Start timer-driven sampling after SD card and sensor initialization
}

void loop() {
  if (sampleRequested) {          // Only read the sensor when the timer interrupt requests a sample.
    noInterrupts();         //clear the interrupt flag
    sampleRequested = false;
    interrupts();

    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature();

    if (isnan(humidity) || isnan(temperatureC)) {       //check if both inputs are numbers
      Serial.println("Error: Failed to read from DHT11 sensor");      //print error message
      digitalWrite(ALERT_LED_PIN, LOW);             //set output LED_PIN as low voltage so it will not light up
      return;
    }

    bool alertOn = temperatureC >= TEMPERATURE_THRESHOLD_C;       //check if surrounding temperature is higher than threshold temperature

    if (alertOn) {
      digitalWrite(ALERT_LED_PIN, HIGH);      //if yes, set alert to HIGH
    } else {
      digitalWrite(ALERT_LED_PIN, LOW);       //if no, set alert to OFF
    }

    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" C");

    Serial.print(" | Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");

    Serial.print(" | Alert: ");
    Serial.println(alertOn ? "ON" : "OFF");

    logSensorData(millis(), temperatureC, humidity, alertOn);
  }

}