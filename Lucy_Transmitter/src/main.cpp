#include <Arduino.h>
#include <SoftwareSerial.h>

#define statusLED 13

SoftwareSerial HC12(11, 10); //RXD, TXD

void HC12Send(int data){
    HC12.println(data);
    digitalWrite(statusLED, 1);
    delay(20);
}

void setup() {
  HC12.begin(9600);
  pinMode(statusLED, OUTPUT);
}

void loop() {
    HC12Send(1234);
}
