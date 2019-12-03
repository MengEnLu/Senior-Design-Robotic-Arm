//========SLAVE CODE========
//========ROBOTIC ARM=======

#define LedPin 9
int incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    incomingByte = Serial.read();
  }
  
  Serial.print("I received: ");
  Serial.println(incomingByte,DEC);
  
  /*if(incomingByte>120){
    digitalWrite(LedPin,HIGH);
  }
  else{
    digitalWrite(LedPin,LOW);
  }
  delay(50);
  */

  analogWrite(LedPin,incomingByte);
  delay(50);

}
