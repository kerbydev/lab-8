# BCA188 - Laboratory 8: Wired Interfaces

## Overview

This laboratory activity focuses on testing wired communication interfaces using an ESP32-S3 development board. It covers three communication protocols: UART, I2C, and SPI. Each interface is tested using a simple program to check if data can be transmitted and received correctly.

## GPIO Pin Configuration

The following GPIO pins are assigned to each communication signal:

| Signal | GPIO Pin |
|---|---|
| UART TX | 17 |
| UART RX | 18 |
| I2C SDA | 8 |
| I2C SCL | 9 |
| SPI SCK | 12 |
| SPI MOSI | 11 |
| SPI MISO | 13 |
| SPI CS | 10 |

## Guide Questions and Answers

### 1. Why must both devices use the same baud rate for UART, while SPI uses a clock line?

UART does not have a separate clock line, so both devices need to use the same baud rate to communicate properly. SPI uses a clock signal to keep the timing of data transmission and reception synchronized.

### 2. Why can't two devices with the same address use the same I2C bus?

Each I2C device has its own address so the controller can identify it. If two devices have the same address, the controller may not be able to tell them apart, which can cause communication problems.

### 3. Why is the CS pin set to HIGH between SPI transfers?

The CS pin is set to HIGH to deselect the SPI device after communication. When CS is set to LOW, the device is selected and ready to communicate with the controller.

## Test Programs

Three programs are used to check the communication interfaces.

### 1. UART Loopback

**File:** `uart_loopback/uart_loopback.ino`

This program sends the text `PING` through the UART TX pin (GPIO 17) and receives data through the UART RX pin (GPIO 18). A jumper wire connects the two pins to test the communication.

### 2. I2C Scanner

**File:** `i2c_scanner/i2c_scanner.ino`

This program scans the I2C bus to find connected devices. It uses GPIO 8 for SDA and GPIO 9 for SCL. The OLED module can be connected to these pins for testing.

### 3. SPI Loopback

**File:** `spi_loopback/spi_loopback.ino`

This program transmits the byte `0xA5` through SPI and reads the received data through the MISO pin. Connecting MOSI to MISO allows the transmitted data to be received back for testing.

## Hardware Requirements

The following components are needed for this laboratory activity:

- ESP32-S3 development board
- I2C OLED module
- Jumper wires
- USB cable for programming and power

## Wiring Connections

### UART Loopback

Connect the UART TX pin to the UART RX pin.

- GPIO 17 (TX) → GPIO 18 (RX)

### SPI Loopback

Connect the SPI MOSI pin to the SPI MISO pin.

- GPIO 11 (MOSI) → GPIO 13 (MISO)

### I2C OLED Connection

Connect the OLED module to the ESP32-S3 using the following pins:

- OLED SDA → GPIO 8
- OLED SCL → GPIO 9
- OLED VCC → Appropriate power pin
- OLED GND → GND

## Expected Results

- **UART:** The transmitted `PING` data should be received through the RX pin when the loopback wiring is correct.
- **I2C:** The scanner should display the addresses of any connected and detected I2C devices.
- **SPI:** The received byte should match the transmitted value `0xA5` when the loopback connection and SPI configuration are correct.

## Conclusion

This laboratory activity demonstrates how UART, I2C, and SPI work using an ESP32-S3 development board. The test programs help verify each interface and show how the correct pin connections and communication settings are important for successful data transmission and reception.
