/*
 * HC12 - ARDUINO
 * VCC - VCC
 * GND - VCC
 * TXD - RX
 * RXD - TX
 * SET - GND
 */
#include <SoftwareSerial.h>

#define RX 7 //Connect to the TX pin of the HC-12
#define TX 6 //Connect to the RX pin of the HC-12

SoftwareSerial mySerial(RX, TX);

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    Serial.println("ON");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
