#define MPU6050_ADDR         0x68
#define MPU6050_ADDR2        0x69 //Added to receive input from second Gyroscope
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"


class MPU6050{
  public:

  MPU6050(TwoWire &w);
  MPU6050(TwoWire &w, float aC, float gC);

  void begin(unsigned char address);

  void setGyroOffsets(float x, float y, float z);

  void writeMPU6050(byte reg, byte data, char address);
  byte readMPU6050(byte reg, char address);

  int16_t getRawAccX(){ return rawAccX; };
  int16_t getRawAccY(){ return rawAccY; };
  int16_t getRawAccZ(){ return rawAccZ; };

  int16_t getRawTemp(){ return rawTemp; };

  int16_t getRawGyroX(){ return rawGyroX; };
  int16_t getRawGyroY(){ return rawGyroY; };
  int16_t getRawGyroZ(){ return rawGyroZ; };

  float getTemp(){ return temp; };

  float getAccX(){ return accX; };
  float getAccY(){ return accY; };
  float getAccZ(){ return accZ; };

  float getGyroX(){ return gyroX; };
  float getGyroY(){ return gyroY; };
  float getGyroZ(){ return gyroZ; };

  void calcGyroOffsets(char address, bool console = false, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);

  float getGyroXoffset(){ return gyroXoffset; };
  float getGyroYoffset(){ return gyroYoffset; };
  float getGyroZoffset(){ return gyroZoffset; };

  void update(unsigned char address);

  float getAccAngleX(){ return angleAccX; };
  float getAccAngleY(){ return angleAccY; };

  float getGyroAngleX(){ return angleGyroX; };
  float getGyroAngleY(){ return angleGyroY; };
  float getGyroAngleZ(){ return angleGyroZ; };

  float getAngleX(){ return angleX; };
  float getAngleY(){ return angleY; };
  float getAngleZ(){ return angleZ; };

  private:

  TwoWire *wire;

  int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
  rawGyroX, rawGyroY, rawGyroZ;

  float gyroXoffset, gyroYoffset, gyroZoffset;

  float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;

  float angleGyroX, angleGyroY, angleGyroZ,
  angleAccX, angleAccY, angleAccZ;

  float angleX, angleY, angleZ;

  float interval;
  long preInterval;

  float accCoef, gyroCoef;
};


MPU6050::MPU6050(TwoWire &w){
  wire = &w;
  accCoef = 0.02f;
  gyroCoef = 0.98f;
}

MPU6050::MPU6050(TwoWire &w, float aC, float gC){
  wire = &w;
  accCoef = aC;
  gyroCoef = gC;
}

void MPU6050::begin(unsigned char address){
  writeMPU6050(MPU6050_SMPLRT_DIV, 0x00, address);
  writeMPU6050(MPU6050_CONFIG, 0x00,address);
  writeMPU6050(MPU6050_GYRO_CONFIG, 0x08,address);
  writeMPU6050(MPU6050_ACCEL_CONFIG, 0x00,address);
  writeMPU6050(MPU6050_PWR_MGMT_1, 0x01,address);
  this->update(address);
  angleGyroX = 0;
  angleGyroY = 0;
  angleX = this->getAccAngleX();
  angleY = this->getAccAngleY();
  preInterval = millis();
}

void MPU6050::writeMPU6050(byte reg, byte data, char address){
  wire->beginTransmission(address);
  wire->write(reg);
  wire->write(data);
  wire->endTransmission();
}

byte MPU6050::readMPU6050(byte reg, char address) {
  wire->beginTransmission(address);
  wire->write(reg);
  wire->endTransmission(true);
  wire->requestFrom(address, 1);
  byte data =  wire->read();
  return data;
}

void MPU6050::setGyroOffsets(float x, float y, float z){
  gyroXoffset = x;
  gyroYoffset = y;
  gyroZoffset = z;
}

void MPU6050::calcGyroOffsets(char address, bool console, uint16_t delayBefore, uint16_t delayAfter){
  float x = 0, y = 0, z = 0;
  int16_t rx, ry, rz;

  delay(delayBefore);
  if(console){
    Serial.println();
    Serial.println("========================================");
    Serial.println("Calculating gyro offsets");
    Serial.print("DO NOT MOVE MPU6050");
  }
  for(int i = 0; i < 3000; i++){
    if(console && i % 1000 == 0){
      Serial.print(".");
    }
    wire->beginTransmission(address);
    wire->write(0x43);
    wire->endTransmission(false);
    wire->requestFrom((int)address, 6);

    rx = wire->read() << 8 | wire->read();
    ry = wire->read() << 8 | wire->read();
    rz = wire->read() << 8 | wire->read();

    x += ((float)rx) / 65.5;
    y += ((float)ry) / 65.5;
    z += ((float)rz) / 65.5;
  }
  gyroXoffset = x / 3000;
  gyroYoffset = y / 3000;
  gyroZoffset = z / 3000;

  if(console){
    Serial.println();
    Serial.println("Done!");
    Serial.print("X : ");Serial.println(gyroXoffset);
    Serial.print("Y : ");Serial.println(gyroYoffset);
    Serial.print("Z : ");Serial.println(gyroZoffset);
    Serial.println("Program will start after 3 seconds");
    Serial.print("========================================");
    delay(delayAfter);
  }
}

void MPU6050::update(unsigned char address){
  wire->beginTransmission(address);
  wire->write(0x3B);
  wire->endTransmission(false);
  wire->requestFrom((int)address, 14);

  rawAccX = wire->read() << 8 | wire->read();
  rawAccY = wire->read() << 8 | wire->read();
  rawAccZ = wire->read() << 8 | wire->read();
  rawTemp = wire->read() << 8 | wire->read();
  rawGyroX = wire->read() << 8 | wire->read();
  rawGyroY = wire->read() << 8 | wire->read();
  rawGyroZ = wire->read() << 8 | wire->read();

  temp = (rawTemp + 12412.0) / 340.0;

  accX = ((float)rawAccX) / 16384.0;
  accY = ((float)rawAccY) / 16384.0;
  accZ = ((float)rawAccZ) / 16384.0;

  angleAccX = atan2(accY, sqrt(accZ * accZ + accX * accX)) * 360 / 2.0 / PI;
  angleAccY = atan2(accX, sqrt(accZ * accZ + accY * accY)) * 360 / -2.0 / PI;

  gyroX = ((float)rawGyroX) / 65.5;
  gyroY = ((float)rawGyroY) / 65.5;
  gyroZ = ((float)rawGyroZ) / 65.5;

  gyroX -= gyroXoffset;
  gyroY -= gyroYoffset;
  gyroZ -= gyroZoffset;

  interval = (millis() - preInterval) * 0.001;

  angleGyroX += gyroX * interval;
  angleGyroY += gyroY * interval;
  angleGyroZ += gyroZ * interval;

  angleX = (gyroCoef * (angleX + gyroX * interval)) + (accCoef * angleAccX);
  angleY = (gyroCoef * (angleY + gyroY * interval)) + (accCoef * angleAccY);
  angleZ = angleGyroZ;

  preInterval = millis();

}

//-----------------------------------------------
//Servo Initializations
//-----------------------------------------------
/*
Servo ShoulderZ;
Servo ShoulderX;
Servo Elbow;
Servo Wrist;
*/

//-----------------------------------------------
//Gyro Init

MPU6050 mpu6050(Wire);
MPU6050 mpu6051(Wire);
// -------------Raw values from Gyroscope------
//Wrist Gyro
double angleX1 = 0; //Directly controls wrist rotation
double angleY1 = 0; //Elbow flexion comparator 1 + angle rotation --> ++angle of elbow(Needs to MULTIPLY and MAP values for correct angle rotations)... Map comparators from 0 to 1 for proportional control
double angleZ1 = 0; //

double angleX2 = 0;
double angleY2 = 0;
double angleZ2 = 0;
//--------------PINOUT for Servos---------------
/*
int ShoulderXpin = 3;
int ShoulderZpin = 5;
int Elbowpin = 6;
int Wristpin = 9;
*/


//----------Flex Sensor Analog Pins------------
const int thumbPin = A0;
const int indexPin = A1;
const int middlePin = A2;
const int rpPin = A3;

//--------------Flex Sensor Angle Variables-----------
int thumb_pos =0;
int index_pos = 0;
int middle_pos = 0;
int rp_pos = 0;

//-----------Flex Sensor User Specified Boudaries-----------
int thumb_max = 0;
int index_max = 0;
int middle_max = 0;
int rp_max = 0;

int thumb_min = 0;
int index_min = 0;
int middle_min = 0;
int rp_min = 0;

//--------- Flexion angles-----------------
int ShoulderZrot= 0;
int ShoulderXrot = 0;
int Elbowrot = 0;
int Wristrot = 0;

//----------Data String-----------------
String data = "";

//-----------LED Init----------------
const int buttonPin = 4;
const int ledPin_setup_mode = 9;
const int ledPin_gyro_init = 8;
const int ledPin_zero_flex_init = 7;
const int ledPin_full_flex_init = 6;
const int ledPin_setup_complete = 5;


void setup() {

  Serial.begin(9600);
  led_init();
  digitalWrite(ledPin_setup_mode,HIGH);
  Serial.println("Entering Setup Mode");

  Wait_to_Continue();

  
  Wire.begin();
  mpu6050.begin('h');
  mpu6051.begin('i');
  mpu6050.calcGyroOffsets(MPU6050_ADDR,true);
  mpu6051.calcGyroOffsets(MPU6050_ADDR2,true);

  Serial.print("Initializing");
  delay(100);
  Serial.print(" . ");
  delay(100);
  Serial.print(" . ");
  delay(100);
  Serial.print(" . ");
  delay(100);
  Serial.println(" . ");
  delay(100);

  digitalWrite(ledPin_gyro_init,HIGH);
  Serial.println("Gyro Init complete");

  //Flex 0 degree Init
  flex_zero_init();
  Serial.println("Flex 0 degree Init Complete");
  delay(500);

  //Flex 180 Degree Init
  full_flex_init();
  Serial.println("Flex 180 degree Init Complete");

  Serial.println("Wireless Serial Communication Established!");
  
  //Setup Complete and Light up led
  digitalWrite(ledPin_setup_complete, HIGH);
  Print_Flex_Boundaries();
  
  Serial.println("ALL Ready!");
}



int wristRotation = 0;
int thetaShoulderRot = 0;
int phiShoulderRot = 0;
int elbowRotation = 0;
void loop() {
 
  //---------------------------------------------------------------
  //Gyroscope Data
  //------------------------------------------------------------
  mpu6050.update('h');//Wrist
  mpu6051.update('i');//Shoulder
  angleX1 = mpu6050.getAngleX(); //angle_1= Figure out the variable type
  angleY1 = mpu6050.getAngleY();
  angleZ1 = mpu6050.getAngleZ();
  angleX2 = mpu6051.getAngleX();
  angleY2 = mpu6051.getAngleY();
  angleZ2 = mpu6051.getAngleZ();
  //------------------------------------------------------

  //Control process
  //------------------------------------------------------
  //---------------Wrist Rotation-----------------------
  Wristrot = (int) angleX1; // Map values
  //Wristrot = map(Wristrot, 180, -180, 0, 250);
  //Wrist.write(Wristrot);
  
  //----------------Elbow-------------------------------
  elbowRotation = (int) (angleY1 - angleZ2); //Map values
  //Elbow.write(Elbowrot);
  
  //----------------Shoulder----------------------------
  ShoulderZrot = (int)(angleY2);// Map values
  //ShoulderZ.write(ShoulderZrot);
  ShoulderXrot = (int)(angleZ2);
  //ShoulderX.write(ShoulderXrot);

  //INSERT FUNCTIONS FOR GETTING FLEX SENSOR VALUE
  getFlexPos();
  
  makeDataString();
  Serial.print(data);
}
//---------------END OF LOOP-------------
//---------------------------------------


//----------------------------------------
//--------------SETUP FUNCTIONS-----------
void led_init(){
  pinMode(buttonPin,INPUT);
  pinMode(ledPin_setup_mode,OUTPUT);
  pinMode(ledPin_gyro_init,OUTPUT);
  pinMode(ledPin_zero_flex_init,OUTPUT);
  pinMode(ledPin_full_flex_init,OUTPUT);
  pinMode(ledPin_setup_complete,OUTPUT);

  digitalWrite(ledPin_setup_mode,LOW);
  digitalWrite(ledPin_gyro_init,LOW);
  digitalWrite(ledPin_zero_flex_init,LOW);
  digitalWrite(ledPin_full_flex_init,LOW);
  digitalWrite(ledPin_setup_complete,LOW);
}


void Wait_to_Continue(){
  int buttonState = 0;
  while(buttonState == LOW){
    buttonState = digitalRead(buttonPin);    
  }
}

void flex_zero_init(){
  int buttonState = 0;
  while(buttonState == LOW){
      buttonState = digitalRead(buttonPin);
      
      if(buttonState == HIGH){
         //INSERT CODE TO SAMPLE 0 FLEX ANALOG VALUE       
          thumb_min = analogRead(thumbPin);
          index_min = analogRead(indexPin);
          middle_min = analogRead(middlePin);
          rp_min = analogRead(rpPin);
          

        //turn on led for 0 flex
        digitalWrite(ledPin_zero_flex_init, HIGH);
      }
  }
}


void full_flex_init(){
  int buttonState = 0;
  while(buttonState == LOW){
    buttonState = digitalRead(buttonPin);
    
    if(buttonState == HIGH){
        //INSERT CODE TO SAMPLE 180 FLEX ANALOG VALUE 
          thumb_max = analogRead(thumbPin);
          index_max = analogRead(indexPin);
          middle_max = analogRead(middlePin);
          rp_max = analogRead(rpPin);

        //turn on led for 180 flex
        digitalWrite(ledPin_full_flex_init,HIGH);
    }
  } 
}


void Print_Flex_Boundaries(){
  Serial.print("thumb_min:\t");Serial.print(thumb_min);
  Serial.print("\tthumb_max:\t");Serial.println(thumb_max);
  Serial.print("index_min:\t");Serial.print(index_min);
  Serial.print("\tindex_max:\t");Serial.println(index_max);
  Serial.print("middle_min:\t");Serial.print(middle_min);
  Serial.print("\tmiddle_max:\t");Serial.println(middle_max);
  Serial.print("rp_min:\t\t");Serial.print(rp_min);
  Serial.print("\trp_max:\t\t");Serial.println(rp_max);
}
//------------END OF SETUP FUNCTIONS------------
//----------------------------------------------




//------------------------------------------------
//--------------------LOOP FUNCTIONS-----------------
void getFlexPos(){
  int thumbVal = analogRead(thumbPin);
  int indexVal = analogRead(indexPin);
  int middleVal = analogRead(middlePin);
  int rpVal = analogRead(rpPin);


  //Checking Boundary Conditions  
  //Make sure analog reading doesn't exceed user specified range
  if(thumbVal <= thumb_min){
    thumbVal = thumb_min;
  }
  else if(thumbVal>= thumb_max){
    thumbVal = thumb_max;
  }

  if(indexVal <= index_min){
    indexVal = index_min;
  }
  else if(indexVal >= index_max){
    indexVal = index_max;
  }

  if(middleVal <= middle_min){
    middleVal = middle_min;
  }
  else if(middleVal >= middle_max){
    middleVal = middle_max;
  }

  if(rpVal <= rp_min){
    rpVal = rp_min;
  }
  else if(rpVal >= rp_max){
    rpVal = rp_max;
  }

  
  //MAPPING FLEX SENSOR VALUE
  thumb_pos =map(thumbVal,thumb_min,thumb_max,0,180);
  index_pos = map(indexVal,index_min,index_max,0,180);
  middle_pos = map(middleVal,middle_min,middle_max,0,180);
  rp_pos = map(rpVal,rp_min,rp_max,0,180);
}


void makeDataString(){
  data = String('<'+ String(thumb_pos)+ '\t'+ String(index_pos)+ '\t'
                    + String(middle_pos)+'\t'+String(rp_pos)+'\t'
                    + String(Wristrot)+ '\t'+ String(elbowRotation)+ '\t'+ String(ShoulderXrot)+ '\t'
                    + String(ShoulderZrot)+ '\t'+ String(0)+ '\t'+ String(0)+ ">\r\n");
}
//--------------END OF LOOP FUNCTIONS--------------
//--------------------------------------------------
