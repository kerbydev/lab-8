BCA188 - Laboratory 8: Wired Interfaces
GPIO Pin Configuration
Signal	GPIO Pin
UART TX	17
UART RX	18
I2C SDA	8
I2C SCL	9
SPI SCK	12
SPI MOSI	11
SPI MISO	13
SPI CS	10

Guide Questions
1. Why does UART require both devices to use the same baud rate, while SPI uses a clock line?

UART does not have a separate clock signal, so both devices must use the same baud rate to ensure proper data transmission and reception. SPI uses a clock line to synchronize the communication between devices.

2. Why can two devices with the same address not share one I2C bus?

Each I2C device is identified by its address. If two devices have the same address, the controller cannot distinguish between them. This may cause both devices to respond to the same command, resulting in communication conflicts.

3. Why is CS set to HIGH between SPI transfers?

The CS (Chip Select) pin is set to HIGH to deactivate the selected SPI device after a transfer is completed. When CS is set to LOW, the device is selected and can communicate with the controller.

Test Programs
UART Loopback (uart_loopback/uart_loopback.ino) - Sends PING through UART TX (GPIO 17) and receives the response through UART RX (GPIO 18).
I2C Scanner (i2c_scanner/i2c_scanner.ino) - Scans for connected I2C devices using SDA (GPIO 8) and SCL (GPIO 9).
SPI Loopback (spi_loopback/spi_loopback.ino) - Sends the byte 0xA5 through SPI and reads back the received data using the configured SPI pins.
Hardware Requirements
ESP32-S3 development board
I2C OLED module for the I2C scanner test
Jumper wires
USB cable for programming and power
Loopback Connections
UART: Connect GPIO 17 (TX) to GPIO 18 (RX).
SPI: Connect GPIO 11 (MOSI) to GPIO 13 (MISO).
I2C: Connect the OLED module's SDA pin to GPIO 8 and SCL pin to GPIO 9. Connect VCC and GND to the appropriate power and ground pins.
