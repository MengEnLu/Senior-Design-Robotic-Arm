#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); // RX | TX on arduino nano
#define LED 9

int value =0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino nano slave ready to use");

  BTSerial.begin(9600);  
  Serial.println("BTserial started at 9600");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  receive();
  //led_test();
}

void receive(){
  if (BTSerial.available()){
    //value = BTSerial.read();
    Serial.write(BTSerial.read());
    //Serial.println(BTSerial.read(),DEC);
  }
    

  if (Serial.available())
     BTSerial.write(Serial.read());
}


void led_test(){
  analogWrite(LED,value);
}
