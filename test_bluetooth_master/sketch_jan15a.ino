String data = "<100\t200\t300\t400\t500\t600\t700\t800\t900\t100>\r\n";
long i = 1; 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int bytesSent = Serial.print(data);
  Serial.print(bytesSent);
  
}

void loop() {
  // put your main code here, to run repeatedly:
   int bytesSent = Serial.print(data);
  Serial.print(bytesSent);
  //delay(1000);
}
