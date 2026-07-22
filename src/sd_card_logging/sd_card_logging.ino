#include "DHT.h"
#include <SPI.h>
#include <SD.h>


#define DHT_PIN 2
#define DHT_TYPE DHT11
#define SD_CS_PIN 10        // SD card chip select pin
#define ALERT_LED_PIN LED_BUILTIN
#define TEMPERATURE_THRESHOLD_C 25.0

DHT dht(DHT_PIN, DHT_TYPE);

const char LOG_FILE_NAME[] = "sensor.csv";      // CSV log file name


volatile bool sampleRequested = false;        // Shared variables between timer interrupt and main loop
volatile unsigned int timerTickCount = 0;

void setupTimer1() {
  // Disable interrupts while configuring Timer1
  noInterrupts();

  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Reset Timer1 counter
  TCNT1 = 0;

  /*
    Configure Timer1 for 1 Hz interrupt.

    Arduino UNO clock speed: 16 MHz
    Prescaler: 1024

    Timer frequency = 16,000,000 / 1024 = 15625 Hz

    To trigger once per second:
    OCR1A = 15625 - 1 = 15624
  */
  OCR1A = 15624;

  // Enable CTC mode: Clear Timer on Compare Match
  TCCR1B |= (1 << WGM12);

  // Set prescaler to 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer1 compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Re-enable interrupts
  interrupts();
}

// Timer1 interrupt service routine.
// This runs automatically once per second.
ISR(TIMER1_COMPA_vect) {
  timerTickCount++;

  // Request one sensor sample every 2 seconds
  if (timerTickCount >= 2) {
    timerTickCount = 0;
    sampleRequested = true;
  }
}

void writeCsvHeaderIfNeeded() {
  // Only write the CSV header if the file does not already exist
  if (!SD.exists(LOG_FILE_NAME)) {
    File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

    if (logFile) {
      logFile.println("time_ms,temperature_c,humidity_percent,alert_status");
      logFile.close();
      Serial.println("CSV header written.");
    } else {
      Serial.println("Error: Could not create sensor.csv");
    }
  }
}

void logSensorData(unsigned long timeMs, float temperatureC, float humidity, bool alertOn) {
  File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

  if (logFile) {
    logFile.print(timeMs);
    logFile.print(",");
    logFile.print(temperatureC);
    logFile.print(",");
    logFile.print(humidity);
    logFile.print(",");
    logFile.println(alertOn ? "ON" : "OFF");

    logFile.close();

    Serial.println("SD log: saved");
  } else {
    Serial.println("Error: Could not open sensor.csv");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);

  Serial.println("Embedded Environmental Data Logger");
  Serial.println("V6 Final: Timer-Driven SD Card Logging Started");

  dht.begin();

  Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error: SD card initialization failed.");
    Serial.println("Check wiring, SD card insertion, and FAT32 formatting.");
    return;
  }

  Serial.println("SD card initialized successfully.");

  writeCsvHeaderIfNeeded();

  // Start timer-driven sampling after SD card and sensor initialization
  setupTimer1();
}

void loop() {
  // Only sample when the timer interrupt requests it
  if (sampleRequested) {
    // Clear the flag safely
    noInterrupts();
    sampleRequested = false;
    interrupts();

    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature();

    if (isnan(humidity) || isnan(temperatureC)) {
      Serial.println("Error: Failed to read from DHT11 sensor");
      digitalWrite(ALERT_LED_PIN, LOW);
      return;
    }

    bool alertOn = temperatureC >= TEMPERATURE_THRESHOLD_C;

    if (alertOn) {
      digitalWrite(ALERT_LED_PIN, HIGH);
    } else {
      digitalWrite(ALERT_LED_PIN, LOW);
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

  // No delay() here.
  // The loop stays free for future tasks.
}