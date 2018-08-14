#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // RXD, TXD

int ledPin = 13;
unsigned long last = millis();//set timer

void setup() {
  HC12.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if(HC12.available() > 1){
    int inputData = HC12.parseInt();

    Serial.print("Input ");
    Serial.println(inputData);

    HC12.flush();
  }
  delay(20);
}
