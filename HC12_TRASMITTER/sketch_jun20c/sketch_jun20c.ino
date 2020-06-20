/* 1byte HC12 TRANSMITTER example.
/* Tutorial link: http://electronoobs.com/eng_arduino_tut96.php
 * Code: http://electronoobs.com/eng_arduino_tut96_code1.php
 * Scheamtic: http://electronoobs.com/eng_arduino_tut96_sch1.php
 * Youtube Channel: http://www.youtube/c/electronoobs   
 * 
  Module // Arduino UNO/NANO    
    GND    ->   GND
    Vcc    ->   3.3V
    Tx     ->   D10
    Rx     ->   D11      
 */
#include <SoftwareSerial.h>
#define RX 7 //Connect to the TX pin of the HC-12
#define TX 6 //Connect to the RX pin of the HC-12

SoftwareSerial HC12(RX, TX);

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}
void loop() {  
  HC12.write('I');      // Send that data to HC-12
  HC12.print(123);      // Send that data to HC-12  
  HC12.write('X');      // Send that data to HC-12
  HC12.print(-92);      // Send that data to HC-12
  HC12.write('Y');      // Send that data to HC-12  
  HC12.write('E');      // Send that data to HC-12  
  delay(100);
}
