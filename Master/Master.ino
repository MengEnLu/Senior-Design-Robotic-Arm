//========MASTER CODE======
//=========Glove=======

#define potPin A0
int potVal = 0;
int outgoingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400); //Default baud rate for bluetooth is set to 9600

/*  Serial.print("I sent out: ");
  Serial.println("0");
  Serial.write(0);

  Serial.print("I sent out: ");
  Serial.println("1");
  Serial.write(1);

  Serial.print("I sent out: ");
  Serial.println("2");
  Serial.write(2);

  Serial.print("I sent out: ");
  Serial.println("3");
  Serial.write(3);

  Serial.println("/////////////////////////");
 */
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potPin);
  outgoingByte = map(potVal,0,1023,0,255);
  Serial.write(outgoingByte);
  delay(50);
  
  /*potVal = analogRead(potPin);

  if(potVal >= 512){
    potVal = 1;
  }
  else{
    potVal = 0;
  }
  
  Serial.print("I sent out: ");
  Serial.println(potVal);
  
  Serial.write(potVal);
  delay(100);
  */
  /*Serial.print("I sent out: ");
  Serial.println("0");
  Serial.write(0);
  delay(100);

  Serial.print("I sent out: ");
  Serial.println("1");
  Serial.write(1);
  delay(100);

  Serial.print("I sent out: ");
  Serial.println("2");
  Serial.write(2);
  delay(100);
*/

 /*
  Serial.write(0);
  delay(50);

  Serial.write(45);
  delay(50);

  Serial.write(100);
  delay(50);

  Serial.write(150);
  delay(50);

  Serial.write(200);
  delay(50);
  
  Serial.write(255);
  delay(50);

  Serial.write(200);
  delay(50);

 Serial.write(150);
 delay(50);

    Serial.write(100);
  delay(50);

    Serial.write(45);
  delay(50);

   Serial.write(0);
  delay(50);
 

  //Serial.println("/////////////////////////");
  */
}
