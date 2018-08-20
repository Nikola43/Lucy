#include <Arduino.h>
#include <SoftwareSerial.h>
#include <MIDI.h>

SoftwareSerial HC12(10, 11); // RXD, TXD

int ledPin = 13;
unsigned long last = millis();//set timer


MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
const int channel = 1;

void sendMidiNote(int note){
    MIDI.sendNoteOn(note, 100, channel);
    delay(200);
    MIDI.sendNoteOff(note, 100, channel);
}

void setup() {
  HC12.begin(9600);
  Serial.begin(9600);
  MIDI.begin();
}

void loop() {
  if(HC12.available() > 1){
    int inputData = HC12.parseInt();

    Serial.print("Input ");
    Serial.println(inputData);

    HC12.flush();
  }

  for (int note=10; note <= 127; note++) {
    sendMidiNote(note);
    delay(500);
  }

  delay(20);
}
