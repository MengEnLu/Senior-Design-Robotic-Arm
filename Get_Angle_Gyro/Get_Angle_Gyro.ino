
//Libraries
//-----------------------------------------------
#include<MPU6050_tockn.h>
#include <Wire.h>
#include <Servo.h>
//-----------------------------------------------
//Servo Initializations
//-----------------------------------------------
Servo index;
Servo middle;
Servo RnP;
Servo Thumb;
//-----------------------------------------------
//Gyro Init

MPU6050 mpu6050(Wire);
MPU6050 mpu6051(Wire);

double angleX1 = 0;
double angleX2 = 0;
double angleY1 = 0;
double angleY2 = 0;
double angleZ1 = 0;
double angleZ2 = 0;

int indexfPin = 0;
int middlefPin = 1;
int RnPfPin = 2;
int thumbPin = 3;


int indexFlex = 0;
int middleFlex = 0;
int RnPFlex = 0;
int thumbFlex = 0;

void setup() {
  //Attaching Servos 
  // CONNECT ONLY TO PWM pins (~#)
  //~3 and ~5 available 
  index.attach(9); 
  middle.attach(10);
  RnP.attach(11);
  Thumb.attach(6);

  
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6051.begin();
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
    Serial.print(" . ");
  delay(100);
}

void loop() {
  //Finger Flexion Direct Control
  //-----------------------------------------------------------------------------------------------
  indexFlex = analogRead(indexfPin);
  indexFlex = map(indexFlex, 0, 1023,0,180); //Calibrate to map(finger, 0,1023, XXX,XXX) to get right rotation direction
  index.write(indexFlex); 
  
  middleFlex = analogRead(middlefPin);
  middleFlex = map(middleFlex, 0, 1023,0,180); //Calibrate to map(finger, 0,1023, XXX,XXX  
  middle.write(middleFlex);
  
  RnPFlex = analogRead(RnPfPin);
  RnPFlex = map(RnPFlex, 0, 1023,0,180);//Calibrate to map(finger, 0,1023, XXX,XXX
  RnP.write(RnPFlex);
  
  thumbFlex = analogRead(thumbPin);
  thumbFlex = map(thumbFlex, 0, 1023,0,180);//Calibrate to map(finger, 0,1023, XXX,XXX
  Thumb.write(thumbFlex);
  //-----------------------------------------------------------------------------------------------
  //Gyroscope Data
  //-----------------------------------------------------------------------------------------------
  mpu6050.update(MPU6050_ADDR);
  mpu6051.update(MPU6050_ADDR2);
  angleX1 = mpu6050.getAngleX(); //angle_1= Figure out the variable type
  angleY1 = mpu6050.getAngleY();
  angleZ1 = mpu6050.getAngleZ();

  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());
  //-----------------------------------------------------------------------------------------------
}
