#include <SoftwareSerial.h>

void setup() {
  
  while(!Serial) {
    
  }
 
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("s");
}

void loop() { // run over and over
  if (Serial1.available()) {
    //Serial.println("SERIAL1");
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    //Serial1.println("SERIAL");
    Serial1.write(Serial.read());
  }
}
