
#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}


// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}




#include <SoftwareSerial.h>
#define RX 2 //Connect to the TX pin of the HC-12
#define TX 3 //Connect to the RX pin of the HC-12
#define MAX_DATA_SIZE 32 //16 bytes

int byteIndex = 0;
char inputBuffer[MAX_DATA_SIZE];
String inputData = "";

//Used variables for receive data and control motors
byte incomingByte;
String readBuffer = "";
String readBufferX = "";
String readBufferY = "";
int X_val = 0;
int Y_val = 0;
int i = 0;
bool x_ready = false;
bool y_ready = false;
bool x_passed = false;
bool data_received = false;
int pwm_x = 0;
int pwm_y = 0;
      int muxChannel = 0;
     int sendMidi = 0;

//SoftwareSerial HC12(RX, TX);

//MIDI channel to use
#define MIDI_CHANNEL 0

//CC slots 14-31 are recognized as assignable controls by MIDI standard
#define MIDI_CC_START 14

#define MUX_NUM_CHANNELS 1


void clearInputBuffer()
{
  for(int i = 0; i < MAX_DATA_SIZE; i++)
    inputBuffer[i] = -1;
}

void printReceivedData()
{
    for(int i = 0; i < MAX_DATA_SIZE; i++)
    {
      Serial.print(inputBuffer[i]);
    }
    Serial.println("");
   
}

void readSerialData()
{
  inputData = "";
  if (Serial1.available() > 0) {
    while(Serial1.available() > 0)
    {
       //inputBuffer[byteIndex] = HC12.read();
       inputData += (char) Serial1.read();
       byteIndex++;
    }
    //printReceivedData();
    Serial.println(inputData);
    byteIndex = 0;
  }


}

void setup() {
  Serial.begin(9600);             // Serial port to computer
  Serial1.begin(9600);

  //HC12.begin(9600);               // Serial port to HC12

while (!Serial) {
    ; // espera hasta que el puerto Serie este conectado. No es necesario para USB nativa.
  }
  Serial.println("on");
      delay(5000);

}




 
void loop() {
  //First we store the entire incoming data into "readBuffer"
  while (Serial1.available()> 0) {             // If the HC-12 has data in
    incomingByte = Serial1.read();             // Store the data byte by byte
    readBuffer += char(incomingByte);       // Add each byte to ReadBuffer total string variable
  }
  delay(5); //This delay has to be equal or higher than the dalay in transmitter

  /*
  We know we first send the X angle data then the Y. So we store the number till
  we receive an "X". If "X" is received we stop adn we then get the x angle data
  till we receive an "Y".
  */
  while (i <= sizeof(readBuffer))
  {
    if(readBuffer[i] == 'X')
    {      x_ready = true;    }
    if(readBuffer[i] == 'Y')
    {      y_ready = true;    }     
    if(!x_ready)
    {      readBufferX = readBufferX + (readBuffer[i] );    }
    if(x_passed && !y_ready)
    {      readBufferY = readBufferY + (readBuffer[i] );    }
    if(x_ready)
    {      x_passed = true;    }
    i=i+1;
  }
  data_received = true;
  
  X_val = readBufferX.toInt(); //Pass the data from string to int so we could use it
  Y_val = readBufferY.toInt();
  
  if(data_received)
  {

    //Now we reset all variables
    readBuffer = "";       //Reset the buffer to empty
    readBufferX = "";       //Reset the buffer to empty
    x_ready = false;        //Reset the other values
    x_passed = false;
    readBufferY = "";       //Reset the buffer to empty
    y_ready = false;  
    i=0; 


   





  }

               if (X_val > 40)
             {
              
                 sendMidi = map(X_val, 40, 100, 0, 127);
    //Serial.print(X_val);
    //Serial.print("  ");
    //Serial.println(sendMidi);
          } 
  controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, sendMidi);

  /*
  while(true){
    int muxChannel = 0;
    
    
    for (byte muxChannel = 0; muxChannel < MUX_NUM_CHANNELS; ++muxChannel)
    {
      Serial.println(i);
        for(int i = 0; i < 127; i++){
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, i);
        delay(10);
        }
        for(int i = 127; i > 0; i--){
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, i);
        delay(10);
        }
    }
            delay(1000);

  }
  */
}
