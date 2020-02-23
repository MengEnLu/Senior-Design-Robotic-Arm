#include <SoftwareSerial.h>
SoftwareSerial BTserial(2,3); //RX/TX on arduino nano
#define potPin A0


int potVal = 0;
//test bluetooth master
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("arduino nano master ready to use");

  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");
}

void loop() {
  // put your main code here, to run repeatedly:
  test();
  //analog_test();
  
}

void test(){
    int i = 48;
    int j = 49;
    int k = 65;
    BTserial.write(i);
    Serial.print("      ");
    Serial.println(i);
    delay(500);
    BTserial.write(j);
    Serial.print("      ");
    Serial.println(j);
    delay(500);
    BTserial.write(k);
    Serial.print("      ");
    Serial.println(k);
    delay(500);
    
}

void analog_test(){
   potVal = analogRead(potPin);
   int brightness = map(potVal,0,1023,0,255);
   BTserial.write(brightness);
   Serial.print(potVal);
   Serial.print("   Sending value:");
   Serial.println(brightness);
   delay(500);
   
   
}
