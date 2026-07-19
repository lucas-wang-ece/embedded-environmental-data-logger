#include "DHT.h"

// DHT11 sensor configuration
#define DHT_PIN 2
#define DHT_TYPE DHT11

// Alert LED configuration
#define ALERT_LED_PIN LED_BUILTIN
#define TEMPERATURE_THRESHOLD_C 25.0

DHT dht(DHT_PIN, DHT_TYPE);

// This flag is set by the timer interrupt and checked in loop().
volatile bool sampleRequested = false;

// Counts 1-second timer ticks.
// When it reaches 2, a new sensor sample is requested.
volatile unsigned int timerTickCount = 0;

void setupTimer1() {
  // Disable interrupts while configuring Timer1.
  noInterrupts();

  // Clear Timer1 control registers.
  TCCR1A = 0;
  TCCR1B = 0;

  // Reset Timer1 counter.
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

  // Enable CTC mode.
  // CTC = Clear Timer on Compare Match.
  TCCR1B |= (1 << WGM12);

  // Set prescaler to 1024.
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer1 compare match interrupt.
  TIMSK1 |= (1 << OCIE1A);

  // Re-enable interrupts.
  interrupts();
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

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);

  Serial.println("Embedded Environmental Data Logger");
  Serial.println("V5: Timer-Driven Sampling Started");

  dht.begin();

  // Start Timer1 after Serial and sensor initialization.
  setupTimer1();
}

void loop() {
  // Only read the sensor when the timer interrupt requests a sample.
  if (sampleRequested) {
    // Safely clear the flag.
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
  }

}
