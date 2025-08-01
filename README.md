# Weather and Environment Monitoring System

## Overview
This project implements a system that collects weather and environmental data using various sensors and displays it in a user-friendly widget interface. The application runs on an HTTP server hosted on a NodeMCU ESP8266 microcontroller. Users connect via a local IP address provided by the Arduino serial monitor and view real-time sensor data through a web browser.

## Features
- Measures and displays temperature, humidity, atmospheric pressure, and precipitation.
- Continuously updates sensor data in a graphical web interface.
- Shows a welcome message reflecting current weather conditions (e.g., "Clear sky", "Partly cloudy", "Rain", "Snow").
- Uses JSON format for data representation and exchange.

## Technologies Used
- NodeMCU ESP8266 IoT platform
- C language for microcontroller firmware
- HTML & CSS for the web interface
- JSON for data serialization

## Input Sensors
- Temperature and humidity sensor (DHT11)
- Atmospheric pressure sensor (BMP280)
- Rain sensor (FC-37)
- Altitude measurement

## System Description
The system operates as an HTTP server on the ESP8266 module, allowing remote access through WiFi. After loading the firmware and connecting to the configured WiFi network, the device provides its local IP via serial monitor. Users can then connect to this IP via a browser to see up-to-date environmental data.

### Sensor Details
- Temperature sensor sensitivity: -40°C to +85°C (±2°C accuracy)
- Rain sensor FC-37 detects water or snow presence by measuring resistance changes.
- Sensors communicate primarily over the I2C interface.

## Usage
1. Upload the firmware to the NodeMCU ESP8266.
2. Connect the device to a power source and ensure WiFi credentials are correctly set.
3. Monitor the serial output for the local IP address.
4. Open a web browser and enter the IP address to access the data interface.

## Personal Contributions
- Main firmware written in C for the microcontroller.
- Web interface developed in HTML.
- Utilized sensor libraries (BMP280, DHT11) for hardware interfacing.
- Designed a scalable and portable monitoring solution powered by battery and WiFi connectivity.

---

