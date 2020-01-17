//intialization --set all sensor value to zero
int thumb_pos =100;
int index_pos = 200;
int middle_pos = 300;
int rp_pos = 400;

int gy1_x = 500;
int gy1_y = 600;
int gy1_z = 700;

int gy2_x = 800;
int gy2_y = 900;
int gy2_z = 100;

String data = "";

//test
long i = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Wireless Serial Communication established!");
  delay(4000);

  //int bytesSent = Serial.print(data);
  //Serial.print(bytesSent);
  
}

void loop() {
  // put your main code here, to run repeatedly:
   //int bytesSent = Serial.print(data);
   //Serial.print(bytesSent);

  makeDataString();
  Serial.print(data);
  i++;
}


void makeDataString(){
  data = String(String(i)+'<'+ String(thumb_pos)+ '\t'+ String(index_pos)+ '\t'
                    + String(middle_pos)+'\t'+String(rp_pos)+'\t'
                    + String(gy1_x)+ '\t'+ String(gy1_y)+ '\t'+ String(gy1_z)+ '\t'
                    + String(gy2_x)+ '\t'+ String(gy2_y)+ '\t'+ String(gy2_z)+ ">\r\n");
  
}
