// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MIDIUSB.write(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MIDIUSB.write(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
  MIDIUSB.write(event);
}

//MIDI channel to use
#define MIDI_CHANNEL 0

//CC slots 14-31 are recognized as assignable controls by MIDI standard
#define MIDI_CC_START 14

#define MUX_NUM_CHANNELS 2


//GND - GND
//VCC - VCC
//SDA - Pin A4
//SCL - Pin A5
 
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
 
const int mpuAddress = 0x68;  //Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;
int gx, gy, gz;
 
void printTab()
{
   Serial.print(F("\t"));
}
 
void printRAW()
{
   Serial.print(F("a[x y z] g[x y z]:t"));
   Serial.print(ax); printTab();
   Serial.print(ay); printTab();
   Serial.print(az); printTab();
   Serial.print(gx); printTab();
   Serial.print(gy); printTab();
   Serial.println(gz);
}

void mapValues(){
  
}


String valores;
 
void setup()
{
   Serial.begin(115200);
   Wire.begin();
   mpu.initialize();
   Serial.println(mpu.testConnection() ? F("IMU iniciado correctamente") : F("Error al iniciar IMU"));
}
 
void loop()
{
  
       mpu.getAcceleration(&ax, &ay, &az);
       mpu.getRotation(&gx, &gy, &gz);
       
        int valueX = map(ax, -32768.0, 32768.0, 0, 255);
        int valueY = map(ay, -32768.0, 32768.0, 0, 255);
        
        int values[2];
        
                int muxChannel = 0;
  
  
    delay(15000);
    for (byte muxChannel = 0; muxChannel < MUX_NUM_CHANNELS; ++muxChannel)
    {
      
      int outValue = 0;
       // Leer las aceleraciones y velocidades angulares
       mpu.getAcceleration(&ax, &ay, &az);
       mpu.getRotation(&gx, &gy, &gz);
       
       //Mostrar los valores por consola
       valores = "90, " +String(ax) + "," + String(ay) + "," + String(az) + ", -90";
       //Serial.println(valores);
       
       
        muxChannel = 0;
        
         valueX = map(ax, 16384.0, -16384.0, 0, 255);
         valueY = map(ay, 16384.0, -16384.0, 0, 255);
        
        values[2];
        
        values[0] = valueX;
        values[1] = valueY;
        
        //rango
        //saltos
        
        #define P_0    0
        #define P_25   64
        #define P_50   128
        #define P_75   192
        #define P_100  255
        
        if (valueX < 0 || valueX < 32)
        {
          outValue = P_0;
        }
        
        if (valueX > 0 && valueX < 64)
        {
          outValue = P_25;
        }
        
        if (valueX > 64 && valueX < 128)
        {
          outValue = P_50;
        }
        
        if (valueX > 128 && valueX < 192)
        {
          outValue = P_75;
        }
        
        if (valueX > 192 && valueX < 255)
        {
          outValue = P_100;
        }
        
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, outValue);
         delay(100);
         
         Serial.println(valueX);
    }
    

    //controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, values[0]);
  
   
  
}


 /*
      
        for(int i = 255; i > 1; i--){
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, i);
        delay(50);
        }
        */

