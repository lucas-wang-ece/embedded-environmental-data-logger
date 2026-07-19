# Embedded Environmental Data Logger

This project is an embedded firmware project built with an Arduino-compatible microcontroller. The goal is to develop a sensor-based environmental data logger with serial debugging, threshold-based alerts, timer-driven sampling, and future SD card logging.

## Project Goal

The system will collect environmental sensor readings, output diagnostic logs through Serial/UART, trigger alerts based on sensor thresholds, and eventually store structured data locally as CSV files.

## Development Progress

- [x] V1: Microcontroller bring-up and built-in LED blink
- [x] V2: Serial monitor output
- [x] V3: Temperature/humidity sensor reading
- [ ] V4: Threshold-based LED/buzzer alert
- [ ] V5: Timer-driven sampling
- [ ] V6: SD card CSV logging

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