//intializating analog pins
const int thumbPin = A0;
const int indexPin = A1;
const int middlePin = A2;
const int rpPin = A3;


//intialization --set all sensor value to zero
int thumb_pos =0;
int index_pos = 0;
int middle_pos = 0;
int rp_pos = 0;

int wrist = 0;
int elbow = 0;
int shoulder_x = 0;
int shoulder_z = 0;

int gy2_y = 0;
int gy2_z = 0;

String data = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Wireless Serial Communication established!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //INSERT FUNCTIONS FOR GETTING FLEX SENSOR VALUE
   getFlexPos();

  //INSERT FUNCTIONS THAT GET GYRO VALUE

  //
  makeDataString();
  Serial.print(data);
}


void getFlexPos(){
  int thumbVal = analogRead(thumbPin);
  int indexVal = analogRead(indexPin);
  int middleVal = analogRead(middlePin);
  int rpVal = analogRead(rpPin);


//MAPPING FLEX SENSOR VALUE
  thumb_pos =map(thumbVal,320,520,-180, 180);
  index_pos = map(indexVal,400,650,-180,180);
  middle_pos = map(middleVal,400,650,-180,180);
  rp_pos = map(rpVal,320,520,-180,180);

}


void makeDataString(){
  data = String('<'+ String(thumb_pos)+ '\t'+ String(index_pos)+ '\t'
                    + String(middle_pos)+'\t'+String(rp_pos)+'\t'
                    + String(wrist)+ '\t'+ String(elbow)+ '\t'+ String(shoulder_x)+ '\t'
                    + String(shoulder_z)+ '\t'+ String(gy2_y)+ '\t'+ String(gy2_z)+ ">\r\n");


}
