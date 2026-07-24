# Embedded Environmental Data Logger

This project is an embedded firmware project built with an Arduino-compatible microcontroller. The goal is to develop a sensor-based environmental data logger with serial debugging, threshold-based alerts, timer-driven sampling, and future SD card logging.

## Project Goal

The system will collect environmental sensor readings, output diagnostic logs through Serial/UART, trigger alerts based on sensor thresholds, and store structured data locally as CSV files on a microSD card

## Development Progress

- [x] V1: Microcontroller bring-up and built-in LED blink
- [x] V2: Serial monitor output
- [x] V3: Temperature/humidity sensor reading
- [x] V4: Threshold-based LED/buzzer alert
- [x] V5: Timer-driven sampling
- [x] V6: SD card CSV logging
- [x] V7: Checksum-based data integrity logging
- [x] V8: Low-power sleep mode

## Hardware

- Arduino-compatible UNO R3 board
- Breadboard
- Jumper wires
- LEDs
- Resistors
- DHT11 temperature/humidity sensor
- Buzzer
- Future: Micro SD card module

## Current Status

V1 has been completed. The Arduino-compatible board was successfully configured in Arduino IDE, and the built-in LED blink firmware was uploaded to verify the development environment.


V2 has been completed. The Arduino-compatible board was successfully configured in Arduino IDE, the built-in LED blink firmware was uploaded, and Serial Monitor output was verified at 9600 baud for basic UART-style debugging.


V3 has been completed. The Arduino-compatible board successfully reads temperature and humidity data from a DHT11 sensor connected to digital pin 2, and outputs real-time sensor readings through Serial Monitor at 9600 baud.


V4 has been completed. The system reads real-time temperature and humidity data from a DHT11 sensor, compares the temperature against a defined threshold, and triggers a built-in LED alert when the threshold is reached. Serial Monitor output shows sensor readings and alert status at 9600 baud.


V5 has been completed. The system uses a hardware timer interrupt to request sensor samples at fixed intervals, replacing delay-based polling. The main loop reads temperature and humidity data from the DHT11 sensor only when a timer-driven sample request flag is set, then updates the LED alert and prints readings through Serial Monitor.


V6 has been completed. The system uses timer-driven sampling to read temperature and humidity data from a DHT11 sensor, updates a threshold-based LED alert, prints diagnostic output through Serial Monitor, and stores structured sensor readings locally on a microSD card as a CSV file using the SPI interface.


V7 has been completed. The system uses timer-driven sampling to read temperature and humidity data from a DHT11 sensor, updates a threshold-based GPIO alert, prints diagnostic output through Serial Monitor, and stores structured CSV sensor data locally on a microSD card. Each logged CSV record includes a checksum field to support basic data integrity verification.

V8 has been completed. The system now enters idle sleep mode between timer-driven sensor samples while Timer1 remains active to wake the MCU. After each wake-up, the system reads DHT11 temperature and humidity data, updates the threshold-based GPIO alert, calculates a checksum, and logs the CSV record to the microSD card.