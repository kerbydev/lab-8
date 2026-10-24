// Sketch 1: UART loopback (Serial2). Jumper TX -> RX.
const int UART_TX = 17;
const int UART_RX = 18;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  Serial.println("UART loopback ready");
}

void loop() {
  if (millis() - lastSend >= 1000) {
    lastSend = millis();
    Serial2.println("PING");
  }
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}