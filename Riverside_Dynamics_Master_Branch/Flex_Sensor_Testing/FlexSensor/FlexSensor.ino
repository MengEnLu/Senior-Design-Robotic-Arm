
int thumbPin = A0;    // select the input pin for the potentiometer
int thumbVal = 0;
int thumbAngle = 0;

int indexPin = A1;   
int indexVal = 0;
int indexAngle = 0;

int middlePin = A2;
int middleVal = 0;
int middleAngle = 0;

int rnpPin = A3;
int rnpVal = 0;
int rnpAngle = 0;


//valid analog reading boundary for 2.2 and 4.5 inch flex
int lower_22flex = 529;
int upper_22flex = 830;

int lower_45flex = 520;
int upper_45flex = 700;

void setup() {
  Serial.begin(9600);
  Serial.println("--------start-----------");
}

void loop() {
  // read the value from the sensor:
  thumbVal = analogRead(thumbPin);
  indexVal = analogRead(indexPin);
  middleVal = analogRead(middlePin);
  rnpVal = analogRead(rnpPin);

  //checking boundary conditions to make sure analog 
  //reading doesn't exceed this range
  //529 to 830 range for 2.2 inch flex sensor
  //520 to 700 range for 4.5 inch flex sensor  
  if(thumbVal <= lower_22flex){
    thumbVal = lower_22flex;
  }
  else if(thumbVal>= upper_22flex){
    thumbVal = upper_22flex;
  }

  if(indexVal <= lower_45flex){
    indexVal = lower_45flex;
  }
  else if(indexVal >= upper_45flex){
    indexVal = upper_45flex;
  }

  if(middleVal <= lower_45flex){
    middleVal = lower_45flex;
  }
  else if(middleVal >= upper_45flex){
    middleVal = upper_45flex;
  }

  if(rnpVal <= lower_22flex){
    rnpVal = lower_22flex;
  }
  else if(rnpVal >= upper_22flex){
    rnpVal = upper_22flex;
  }
  
  thumbAngle = map(thumbVal,lower_22flex,upper_22flex,0,180);
  indexAngle = map(indexVal,lower_45flex, upper_45flex,0,180);
  middleAngle = map(middleVal,lower_45flex,upper_45flex,0,180);
  rnpAngle = map(rnpVal, lower_22flex, upper_22flex,0,180);
  //Serial.print("thumb angle: ");
  //Serial.println(thumbAngle);
  //Serial.print("index: ");
  //Serial.println(indexAngle);
  //Serial.print("middle: ");
  //Serial.println(middleAngle);
  //Serial.print("rnp: ");
  //Serial.println(rnpAngle);
  
  //delay(50);
}
