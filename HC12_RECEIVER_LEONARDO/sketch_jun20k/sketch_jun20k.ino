#include <SoftwareSerial.h>
#define RX 2 //Connect to the TX pin of the HC-12
#define TX 3 //Connect to the RX pin of the HC-12
#define MAX_DATA_SIZE 32 //16 bytes

int byteIndex = 0;
char inputBuffer[MAX_DATA_SIZE];
String inputData = "";

//SoftwareSerial HC12(RX, TX);

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
}
void loop() {
  readSerialData();
  delay(20); 
}
