void setup() {
  Serial.begin(9600);            // Start serial communication at 9600 baud.

  pinMode(LED_BUILTIN, OUTPUT);      // Configure the built-in LED pin as an output pin.

  Serial.println("Embedded Environmental Data Logger");       //Output starting message when the board is reset or connect
  Serial.println("V2: Serial Monitor Output Started");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);      //set the LED pin as high voltage
  Serial.println("System running... LED ON");
  delay(1000);              //wait for 1 second

  digitalWrite(LED_BUILTIN, LOW);       //set the LED pin as low voltage
  Serial.println("System running... LED OFF");
  delay(1000);              //wait for 1 second
}