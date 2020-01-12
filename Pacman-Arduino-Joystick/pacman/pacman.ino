#include "Wire.h"

byte _buff[6];
char POWER_CTL = 0x2D;
char DATA_FORMAT = 0x31;
const int DEVICE_ADDRESS = (0x53);
char DATAX0 = 0x32;  //X-Axis Data 0
char DATAX1 = 0x33; //X-Axis Data 1
char DATAY0 = 0x34; //Y-Axis Data 0
char DATAY1 = 0x35; //Y-Axis Data 1
char DATAZ0 = 0x36; //Z-Axis Data 0
char DATAZ1 = 0x37; //Z-Axis Data 1  

struct data 
{
  float x, y, z;
};

data data;
float previous_x = 0;
float previous_y = 0;
float previous_z = 0;

//pos inicial;
// x incrementa LEFT   y neg
// x decrementa RIGHT  y neg
// y incrementa UP x neg
// y decrementa DOWN x pos

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);    
  Wire.begin();
  writeTo(DEVICE_ADDRESS, DATA_FORMAT, 0x01); //Poner ADXL345 en +- 4G
  writeTo(DEVICE_ADDRESS, POWER_CTL, 0x08);  //Poner el ADXL345  
  //delay(2000);
  //Serial.println("Vergggota");
}

void loop() {
  // put your main code here, to run repeatedly:
  float x, y, z;  
  readAccel(x, y, z);
  int posX, posY;

  data.x = x;
  data.y = y;
  data.z = z;
  if(previous_x == 0 || previous_y == 0 || previous_z == 0){
    previous_x = x;
    previous_y = y;
    previous_z = z;
  } else {
    if((abs(x) - abs(previous_x)) < (abs(y) - abs(previous_z)) ){
      /*if(y < previous_y && 2 < (abs(y) - abs(previous_z))){
        Serial.println((char*) "DOWN");
      } else if (y > previous_y && 2 < (abs(y) - abs(previous_z))){
        Serial.println((char*) "UP");
      }*/
      if(x > 0 && y < 0){
        Serial.println((char*) "DOWN");
      } else if (x < 0 && y > 0) {
        Serial.println((char*) "UP");
      } /*else {
        Serial.println((char*) "NN");
      }*/
    } else {
      if(x < previous_x && 5 < (abs(x) - abs(previous_x))){
        Serial.println((char*) "RIGHT");
      } else if (x > previous_x && 5 < (abs(x) - abs(previous_x))) {
        Serial.println((char*) "LEFT");
      } /*else {
        Serial.println((char*) "NN");
      }*/
    }
    /*if(x < 0 && y < 0){
       Serial.println((char*) "RIGHT");
    } else if (x > 0 && y < 0) {
      Serial.println((char*) "LEFT");
    }*/
    previous_x = x;
    previous_y = y;
    previous_z = z;
  }
  delay(200);
  /*Serial.write((char*) "Accelerometer:");
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y); 
  Serial.print(" z: ");
  Serial.print(z);
  Serial.print("\n");*/
  //delay(1000);*/
  
}

void writeTo(int device, byte address, byte val) 
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readAccel(float &x, float &y, float &z) {
  //Leer los datos
  
  uint8_t numBytesToRead = 6;
  readFrom(DEVICE_ADDRESS, DATAX0, numBytesToRead, _buff);

  //Leer los valores del registro y convertir a int (Cada eje tiene 10 bits, en 2 Bytes LSB)
  x = (((int)_buff[1]) << 8) | _buff[0];   
  y = (((int)_buff[3]) << 8) | _buff[2];
  z = (((int)_buff[5]) << 8) | _buff[4];
}

void readFrom(int device, byte address, int num, byte buff[])
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();

  Wire.beginTransmission(device);
  Wire.requestFrom(device, num);

  int i = 0;
  while(Wire.available())
  {
    buff[i++] = Wire.read();  
  }
  
  Wire.endTransmission();
}
