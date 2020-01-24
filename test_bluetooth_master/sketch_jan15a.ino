//intialization --set all sensor value to zero
int thumb_pos =1;
int index_pos = 2;
int middle_pos = 3;
int rp_pos = 4;

int gy1_x = 5;
int gy1_y = 6;
int gy1_z = 7;

int gy2_x = 8;
int gy2_y = 9;
int gy2_z = 10;

String data = "";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Wireless Serial Communication established!");
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

  thumb_pos ++;
  index_pos ++;
  middle_pos ++;
  rp_pos ++;

  gy1_x ++;
  gy1_y ++;
  gy1_z ++;

  gy2_x ++;
  gy2_y ++;
  gy2_z ++;

}

void getFlexData(){
  
}

void getGyData(){

}

void makeDataString(){
  data = String('<'+ String(thumb_pos)+ '\t'+ String(index_pos)+ '\t'
                    + String(middle_pos)+'\t'+String(rp_pos)+'\t'
                    + String(gy1_x)+ '\t'+ String(gy1_y)+ '\t'+ String(gy1_z)+ '\t'
                    + String(gy2_x)+ '\t'+ String(gy2_y)+ '\t'+ String(gy2_z)+ ">\r\n");
  
}
