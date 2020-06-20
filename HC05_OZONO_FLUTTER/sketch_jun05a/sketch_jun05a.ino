#include <SoftwareSerial.h> 
#define MAX_DATA_SIZE 16 //16 bytes
#define BL_RX_PIN 2
#define BL_TX_PIN 3
#define REST_PIN 4
#define ON_OFF_PIN 5
#define WORK_PIN 6



SoftwareSerial MyBlue(BL_RX_PIN, BL_RX_PIN); // RX | TX 

int flag = 0; 
int byteIndex = 0;
char inputBuffer[MAX_DATA_SIZE];

void clearInputBuffer()
{
  for(int i = 0; i < MAX_DATA_SIZE; i++)
    inputBuffer[i] = 0;
}

void printReceivedData()
{
  if (inputBuffer[0] != 0) 
  {
    for(int i = 0; i < MAX_DATA_SIZE; i++)
    {
      Serial.print(inputBuffer[i]);
    }
    Serial.println();
  }
}

void readBluetoothData()
{
  while(MyBlue.available())
  {
    clearInputBuffer();
    inputBuffer[byteIndex] = MyBlue.read();
    byteIndex++;
  }
  byteIndex = 0;
  flag = inputBuffer[0];
}

void setup() 
{   
 pinMode(REST_PIN, OUTPUT);
 pinMode(ON_OFF_PIN, OUTPUT);
 pinMode(WORK_PIN, OUTPUT);
 
 digitalWrite(REST_PIN, HIGH); 
 digitalWrite(ON_OFF_PIN, HIGH); 
 digitalWrite(WORK_PIN, HIGH); 


 clearInputBuffer();
 
 Serial.begin(9600); 
 MyBlue.begin(9600); 
 // while (!Serial); // arduino leonardo
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
} 
void loop() 
{ 
  readBluetoothData();
  printReceivedData();
  
 switch(flag) {
  case '1' :
     digitalWrite(WORK_PIN, LOW); 
     delay(200);
     digitalWrite(WORK_PIN, HIGH); 
  break;
  case '2' : 
     digitalWrite(REST_PIN, LOW); 
     delay(200);
     digitalWrite(REST_PIN, HIGH);
     break;
  case '3' : 
     digitalWrite(ON_OFF_PIN, LOW); 
     delay(200);
     digitalWrite(ON_OFF_PIN, HIGH); 
     break;
}
}  
