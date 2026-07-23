#include "DHT.h"

// DHT11 sensor configuration
#define DHT_PIN 2
#define DHT_TYPE DHT11

// Alert LED configuration
#define ALERT_LED_PIN LED_BUILTIN       // Use the built-in LED as the alert indicator.
#define TEMPERATURE_THRESHOLD_C 25.0    //define threshold temperature

DHT dht(DHT_PIN, DHT_TYPE);       // Create a DHT sensor object using the selected pin and sensor type.


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

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT);     //select the alert LED as the output pin

  Serial.println("Embedded Environmental Data Logger");       //print startup message
  Serial.println("V5: Timer-Driven Sampling Started");

  dht.begin();              //initialize the DHT11 sensor

  setupTimer1();            // Start Timer1 after Serial and sensor initialization.
}

void loop() {
  if (sampleRequested) {        // Only read the sensor when the timer interrupt requests a sample.

    noInterrupts();             //clear the interrupt flag
    sampleRequested = false;
    interrupts();

    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature();

    if (isnan(humidity) || isnan(temperatureC)) {         //check if both inputs are numbers
      Serial.println("Error: Failed to read from DHT11 sensor");      //print error message
      digitalWrite(ALERT_LED_PIN, LOW);                   //set output LED_PIN as low voltage so it will not light up
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" C");

    Serial.print(" | Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");

    if (temperatureC >= TEMPERATURE_THRESHOLD_C) {         //check if surrounding temperature is higher than threshold temperature
      digitalWrite(ALERT_LED_PIN, HIGH);
      Serial.println(" | Alert: ON");               //if yes, set alert to HIGH
    } else {
      digitalWrite(ALERT_LED_PIN, LOW);
      Serial.println(" | Alert: OFF");              //if no, set alert to OFF
    }
  }

}
