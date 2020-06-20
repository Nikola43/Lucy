#include <SoftwareSerial.h> 
#include <EEPROM.h>
#define BL_RX_PIN 8
#define BL_TX_PIN 9
#define BLUE_PIN 3
#define GREEN_PIN 5
#define RED_PIN 6
#define MAX_DATA_SIZE 8 //16 bytes

int byteIndex = 0;
char inputBuffer[MAX_DATA_SIZE];

char redValue = 0;
char greenValue = 0;
char blueValue = 0;

SoftwareSerial MyBlue(BL_RX_PIN, BL_RX_PIN); // RX | TX 

void readBluetoothData()
{
  if( MyBlue.available() > 0 )
  {
     while(MyBlue.available() > 0)
     {
        inputBuffer[byteIndex] = MyBlue.read();
        byteIndex++;
     }
     hexToRGB(inputBuffer);
     writeColor();
     //printReceivedData();
     writeColorEEPROM();
  }
  byteIndex = 0;
}

void writeColorEEPROM() {
   EEPROM.write(0, redValue);
   EEPROM.write(1, greenValue);
   EEPROM.write(2, blueValue);
}

void readColorEEPROM() {
   redValue = EEPROM.read(0);
   greenValue = EEPROM.read(1);
   blueValue = EEPROM.read(2);
}

void readSerialData()
{
  if( Serial.available() > 0)
  {
     clearInputBuffer();
     while(Serial.available() > 0)
     {
        inputBuffer[byteIndex] = Serial.read();
        byteIndex++;
     }
     hexToRGB(inputBuffer);
     writeColor();
     //printReceivedData();
     writeColorEEPROM();
  }
  byteIndex = 0;
}

void hexToRGB(char *hex) {

int r, g, b;
sscanf(hex, "%02x%02x%02x", &r, &g, &b);


  
  redValue =  r;
  greenValue =  g;
  blueValue = b;

  Serial.print((int)r);
  Serial.print((int)g);
  Serial.println((int)b);
}

void writeColor() {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}

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

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);

  Serial.begin(9600); //This pipes to the serial monitor
  MyBlue.begin(9600); 
  //while(!Serial);
  Serial.println("ON");
  readColorEEPROM();
  writeColor();
}
void loop() {
  readSerialData();
  readBluetoothData();
  delay(100);
}
