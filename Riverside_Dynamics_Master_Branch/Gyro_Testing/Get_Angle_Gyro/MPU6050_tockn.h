
#include "Arduino.h"
#include "Wire.h"
//#include "MPU6050_tockn.cpp"

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

class MPU6050{
  public:

  MPU6050(TwoWire &w);
  MPU6050(TwoWire &w, float aC, float gC);

  void begin(char address);

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

void MPU6050::begin(char address){
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
  mpu6050.update();
  mpu6051.update();
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
