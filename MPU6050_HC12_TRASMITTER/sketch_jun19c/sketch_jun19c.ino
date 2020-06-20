/* YouTube channel. http://www.youtube.com/c/electronoobs * 
 * This is an example where get the data of the MPU6050
 * and send it using the HC12 radio module. 
 * Schematic: https://www.electronoobs.com/eng_arduino_tut44_sch1.php
 * Tutorial: https://www.electronoobs.com/eng_arduino_tut44.php
 */
//Libraries
#include <Wire.h>
#include <SoftwareSerial.h>
#define RX 7 //Connect to the TX pin of the HC-12
#define TX 6 //Connect to the RX pin of the HC-12

SoftwareSerial HC12(RX, TX);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Inputs
int Joy_x = A0;
int Joy_y = A1;


//Variables for the Gyro data
float elapsedTime, time, timePrev;        //Variables for time control
int gyro_error=0;                         //We use this variable to only calculate once the gyro data error
float Gyr_rawX, Gyr_rawY, Gyr_rawZ;     //Here we store the raw data read 
float Gyro_angle_x, Gyro_angle_y;         //Here we store the angle value obtained with Gyro data
float Gyro_raw_error_x, Gyro_raw_error_y; //Here we store the initial gyro data error

//Variables for acceleration data
int acc_error=0;                         //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180/3.141592654;      //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read 
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error
//Total angle data
float Total_angle_x, Total_angle_y;
//Data to send
int x_send, y_send, joy_x_send, joy_y_send;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(Joy_x,INPUT);
  pinMode(Joy_y,INPUT);
  Serial.begin(9600);                     //Start serial com in order to print valeus on monitor
  HC12.begin(9600);                       //Start the HC12 serial communication at 9600 bauds. Change bauds if needed
  begin_MPU6050();                        //Call this function and start the MPU6050 module  
  time = millis();                        //Start counting time in milliseconds
  calculate_IMU_error();                  //Call this function and het the acc and gyro error at the beginning
}//end of setup void


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {  
  timePrev = time;                            // the previous time is stored before the actual time read
  time = millis();                            // actual time read
  elapsedTime = (time - timePrev) / 1000;     //divide by 1000 in order to obtain seconds

  //////////////////////////////////////Gyro read/////////////////////////////////////

  Wire.beginTransmission(0x68);             //begin, Send the slave adress (in this case 68) 
  Wire.write(0x43);                          //First adress of the Gyro data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,4,true);             //We ask for just 4 registers        
  Gyr_rawX=Wire.read()<<8|Wire.read();      //Once again we shif and sum
  Gyr_rawY=Wire.read()<<8|Wire.read();
  /*Now in order to obtain the gyro data in degrees/seconds we have to divide first
  the raw value by 32.8 because that's the value that the datasheet gives us for a 1000dps range*/
  /*---X---*/
  Gyr_rawX = (Gyr_rawX/32.8) - Gyro_raw_error_x; 
  /*---Y---*/
  Gyr_rawY = (Gyr_rawY/32.8) - Gyro_raw_error_y;    
  /*Now we integrate the raw value in degrees per seconds in order to obtain the angle
  * If you multiply degrees/seconds by seconds you obtain degrees */
  /*---X---*/
  Gyro_angle_x = Gyr_rawX*elapsedTime;
  /*---X---*/
  Gyro_angle_y = Gyr_rawY*elapsedTime;
        
  
  //////////////////////////////////////Acc read/////////////////////////////////////
  
  Wire.beginTransmission(0x68);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3B);                 //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(0x68,6,true);    //We ask for next 6 registers starting withj the 3B  
  /*We have asked for the 0x3B register. The IMU will send a brust of register.
  * The amount of register to read is specify in the requestFrom function.
  * In this case we request 6 registers. Each value of acceleration is made out of
  * two 8bits registers, low values and high values. For that we request the 6 of them  
  * and just make then sum of each pair. For that we shift to the left the high values 
  * register (<<) and make an or (|) operation to add the low values.
  If we read the datasheet, for a range of+-8g, we have to divide the raw values by 4096*/    
  Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
  Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
  Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ; 
  /*Now in order to obtain the Acc angles we use euler formula with acceleration values
  after that we substract the error value found before*/  
  /*---X---*/
  Acc_angle_x = (atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_x;
  /*---Y---*/
  Acc_angle_y = (atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_y;    

  //////////////////////////////////////Total angle and filter/////////////////////////////////////
  /*---X axis angle---*/
  Total_angle_x = 0.98 *(Total_angle_x + Gyro_angle_x) + 0.02*Acc_angle_x;
  /*---Y axis angle---*/
  Total_angle_y = 0.98 *(Total_angle_y + Gyro_angle_y) + 0.02*Acc_angle_y;

   


  joy_x_send = analogRead(Joy_x);
  joy_y_send = analogRead(Joy_y);
  //x_send = (int) -Total_angle_x;  //pass from flaot to int in order to send the values, also invert X angle
  //y_send = (int) Total_angle_y;   //pass from flaot to int in order to send the values

  x_send = static_cast<int>(-Total_angle_x);
  y_send = static_cast<int>(Total_angle_x);



  HC12.print(x_send);HC12.write("X"); //Send the x angle value and then the "X" character
  HC12.print(y_send);HC12.write("Y"); //Send the y angle value and then the "Y" character
  delay(20); //Add a delay of 50. This might affect the transmission 
   //Uncoment the next lines if you want to print the values on the serial monitor  
   Serial.print("Xº: ");
   Serial.print(x_send);
   Serial.print("   |   ");
   Serial.print("Yº: ");
   Serial.print(y_send);
   Serial.println(" ");
}//End of void loop



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void begin_MPU6050()
{
  Wire.begin();                           //begin the wire comunication    
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68)              
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission
  //Gyro config
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68) 
  Wire.write(0x1B);                       //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                       //Set the register bits as 00010000 (1000dps full scale)
  Wire.endTransmission(true);             //End the transmission with the gyro
  //Acc config
  Wire.beginTransmission(0x68);           //Start communication with the address found during search.
  Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
  Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true); 
}


void calculate_IMU_error()
{
  /*Here we calculate the acc data error before we start the loop
 * I make the mean of 200 values, that should be enough*/
  if(acc_error==0)
  {
    for(int a=0; a<200; a++)
    {
      Wire.beginTransmission(0x68);
      Wire.write(0x3B);                       //Ask for the 0x3B register- correspond to AcX
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,6,true); 
      
      Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
      Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
      Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ;

      
      /*---X---*/
      Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg));
      /*---Y---*/
      Acc_angle_error_y = Acc_angle_error_y + ((atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg)); 
      
      if(a==199)
      {
        Acc_angle_error_x = Acc_angle_error_x/200;
        Acc_angle_error_y = Acc_angle_error_y/200;
        acc_error=1;
      }
    }
  }//end of acc error calculation   


/*Here we calculate the gyro data error before we start the loop
 * I make the mean of 200 values, that should be enough*/
  if(gyro_error==0)
  {
    for(int i=0; i<200; i++)
    {
      Wire.beginTransmission(0x68);            //begin, Send the slave adress (in this case 68) 
      Wire.write(0x43);                        //First adress of the Gyro data
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,4,true);           //We ask for just 4 registers 
         
      Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shif and sum
      Gyr_rawY=Wire.read()<<8|Wire.read();
   
      /*---X---*/
      Gyro_raw_error_x = Gyro_raw_error_x + (Gyr_rawX/32.8); 
      /*---Y---*/
      Gyro_raw_error_y = Gyro_raw_error_y + (Gyr_rawY/32.8);
      if(i==199)
      {
        Gyro_raw_error_x = Gyro_raw_error_x/200;
        Gyro_raw_error_y = Gyro_raw_error_y/200;
        gyro_error=1;
      }
    }
  }//end of gyro error calculation   
}
