//Mega Serial 1 TX1 pin 18
//Mega Serial 1 RX1 pin 19
#include "Servo.h"

int dataArray[] ={0,0,0,0,0,0,0,0,0,0};
int thumb_pos =dataArray[0];
int index_pos = dataArray[1];
int middle_pos = dataArray[2];
int rp_pos = dataArray[3];

int wrist = dataArray[4];
int elbow = dataArray[5];
int shoulder_x = dataArray[6];
int shoulder_z = dataArray[7];

int gy2_y = dataArray[8];
int gy2_z = dataArray[9];

//--servo----
Servo Thumb;
Servo Index;
Servo Middle;
Servo RP;
Servo Wrist;
Servo Elbow;
Servo Shoulder_x;
Servo Shoulder_z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino Mega slave ready to use");

  Serial1.begin(9600);  
  Serial.println("Arduino Mega serial 1 started at 9600");
  bluetoothSetUp();

//--servo---
  Thumb.attach(3);
  Index.attach(4);
  Middle.attach(5);
  RP.attach(6);
  Wrist.attach(7);
  Elbow.attach(8);
  Shoulder_x.attach(9);
  Shoulder_z.attach(10);
  Serial.println("Servo Ready");
  
  Serial.println("ALL Ready");

}

void loop() {
  // put your main code here, to run repeatedly:
  String tempStr = "";
  String dataStr = "";
  bool dataReady = false;

  while(dataReady == false){
    tempStr = receive();
    //test
    //Serial.print("Incomplete: "); 
    //Serial.println(tempStr);
    if(findDataStr(tempStr) == true){
      dataStr = getDataStr(tempStr);
      dataReady = true;
    }
  }
  //test
  //Serial.print("data String Ready: ");
  //Serial.println(dataStr);

  
  //INSERT FUNCTIONS TO EXTRACT DATA
  extractData(dataStr);
  assignVar();
  
  //test extract Data function
  test_Print_Extracted_Data();
  
  //INSERT FUNCTIONS FOR SERVOS AND MOTORS HERE
  Thumb.write(thumb_pos);
  delay(15);
  Index.write(index_pos);
  delay(15);
  Middle.write(middle_pos);
  delay(15);
  RP.write(rp_pos);
  delay(15);
  
  Wrist.write(wrist);
  delay(15);
  Elbow.write(elbow);
  delay(15);
  Shoulder_x.write(shoulder_x);
  delay(15);
  Shoulder_z.write(shoulder_z);
  delay(15);
    
  clearBuffer();
  
}


//this function is used *ONLY* during SETUP
void bluetoothSetUp(){
  while(Serial1.available()){
    Serial.print(Serial1.read());
  }
}


String receive(){

  String str = "";
  bool flag = false;

  while(flag == false){
    String tempStr ="";
    if(Serial1.available()){
      tempStr = Serial1.readStringUntil('>');
    }

    int k = tempStr.length();
    if(k>=21 && k<=80){
      str = tempStr;
      flag = true;
    }
  }
  
  return str;
}

bool findDataStr(String tempStr){
  bool flag = false;
  for(int i = 0; i< tempStr.length(); i++){
    if(tempStr.charAt(i)== '<'){
      flag = true;
      break;
    }
  }
  return flag;
}

String getDataStr(String tempStr){
  int i = tempStr.indexOf('<');
  tempStr = tempStr.substring(i);
  tempStr = tempStr + '>';
  
  /*int k = tempStr.indexOf('>');
        //test
  Serial.print("tempStr.length()-1: ");
  Serial.println(tempStr.length()-1);
  
  if (k <= tempStr.length()-1){
    tempStr = tempStr.substring(0,k+1);
  }
  else{    
    tempStr = tempStr.substring(0,k);
    tempStr = tempStr + '>';
  }
  */
  return tempStr;
}

void assignVar(){
  thumb_pos =dataArray[0];
  index_pos = dataArray[1];
  middle_pos = dataArray[2];
  rp_pos = dataArray[3];

  wrist = dataArray[4];
  elbow = dataArray[5];
  shoulder_x = dataArray[6];
  shoulder_z = dataArray[7];
  
  gy2_y = dataArray[8];
  gy2_z = dataArray[9];
}

void extractData(String dataStr){
  //int tempArray[10];
  int start = 1;
  int e = -1;
  int counter = 0;
  //Serial.print("tempArray Size");
  //Serial.println(sizeof(tempArray));
  for(int i = 1; i <= dataStr.length()-1; i++){
    if((dataStr.charAt(i) == '\t') || (dataStr.charAt(i) == '>')){
      e = i;
      String temp = dataStr.substring(start,e);
      //test
      //Serial.println(temp);
      dataArray[counter] = temp.toInt();
      //test
      /*Serial.print("counter\t");
      Serial.print(counter);
      Serial.print("\tdataArrayVal:\t");
      Serial.println(dataArray[counter]);
      */   
      counter++;
      start = i+1;
      //test
      //Serial.println(i);
    }
  }
  //Serial.print("tempArray Size: ");
  //Serial.println(sizeof(tempArray));
}



void test_Print_Extracted_Data(){
  Serial.print("thumb: ");
  Serial.print(thumb_pos);
  Serial.print("\tindex: ");
  Serial.print(index_pos);
  Serial.print("\tmiddle: ");
  Serial.print(middle_pos);
  Serial.print("\trp: ");
  Serial.print(rp_pos);
  Serial.print("\twrist: ");
  Serial.print(wrist);
  Serial.print("\telbow: ");
  Serial.print(elbow);
  Serial.print("\tshoulder_x: ");
  Serial.print(shoulder_x);
  Serial.print("\tshoulder_z: ");
  Serial.print(shoulder_z);
  
  Serial.print("\tgy2_y: ");
  Serial.print(gy2_y);
  Serial.print("\tgy2_z: ");
  Serial.println(gy2_z);
}


void clearBuffer(){
  while(Serial1.available()){
    Serial1.read();
  }
}
