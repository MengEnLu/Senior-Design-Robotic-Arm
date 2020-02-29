//----------Flex Sensor Analog Pins------------
const int thumbPin = A0;
const int indexPin = A1;
const int middlePin = A2;
const int rpPin = A3;

//----led init test part global variable-----
const int buttonPin = 4;
const int ledPin_gyro_init = 8;
const int ledPin_zero_flex_init = 7;
const int ledPin_full_flex_init = 6;
const int ledPin_setup_complete = 5;

int thumb_max = 0;
int index_max = 0;
int middle_max = 0;
int rp_max = 0;

int thumb_min = 0;
int index_min = 0;
int middle_min = 0;
int rp_min = 0;
//------END of global variable------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  led_init();
  
  //gyro init
  delay(400);
  digitalWrite(ledPin_gyro_init,HIGH);
  Serial.println("gyro init complete");

  //flex 0 degree init
  flex_zero_init();
  Serial.println("flex 0 degree init complete");
  delay(500);

  
  //flex 180 degree init
  full_flex_init();
  Serial.println("flex 180 degree init complete");

  //setup complete and light up led
  digitalWrite(ledPin_setup_complete, HIGH);

  Print_Flex_Boundaries();
  Serial.println("ALL Initialization Complete");
}



void loop() {
  // put your main code here, to run repeatedly:

}





void led_init(){
  pinMode(buttonPin,INPUT);
  pinMode(ledPin_gyro_init,OUTPUT);
  pinMode(ledPin_zero_flex_init,OUTPUT);
  pinMode(ledPin_full_flex_init,OUTPUT);
  pinMode(ledPin_setup_complete,OUTPUT);

  digitalWrite(ledPin_gyro_init,LOW);
  digitalWrite(ledPin_zero_flex_init,LOW);
  digitalWrite(ledPin_full_flex_init,LOW);
  digitalWrite(ledPin_setup_complete,LOW);
}


void flex_zero_init(){
  int buttonState = 0;
  while(buttonState == LOW){
      buttonState = digitalRead(buttonPin);
      
      if(buttonState == HIGH){
         //INSERT CODE TO SAMPLE 0 FLEX ANALOG VALUE       
          thumb_min = analogRead(thumbPin);
          index_min = analogRead(indexPin);
          middle_min = analogRead(middlePin);
          rp_min = analogRead(rpPin);
          

        //turn on led for 0 flex
        digitalWrite(ledPin_zero_flex_init, HIGH);
      }
  }
}


void full_flex_init(){
  int buttonState = 0;
  while(buttonState == LOW){
    buttonState = digitalRead(buttonPin);
    
    if(buttonState == HIGH){
        //INSERT CODE TO SAMPLE 180 FLEX ANALOG VALUE 
          thumb_max = analogRead(thumbPin);
          index_max = analogRead(indexPin);
          middle_max = analogRead(middlePin);
          rp_max = analogRead(rpPin);

        //turn on led for 180 flex
        digitalWrite(ledPin_full_flex_init,HIGH);
    }
  } 
}


void Print_Flex_Boundaries(){
  Serial.print("thumb_min:\t");Serial.print(thumb_min);
  Serial.print("\tthumb_max:\t");Serial.println(thumb_max);
  Serial.print("index_min:\t");Serial.print(index_min);
  Serial.print("\tindex_max:\t");Serial.println(index_max);
  Serial.print("middle_min:\t");Serial.print(middle_min);
  Serial.print("\tmiddle_max:\t");Serial.println(middle_max);
  Serial.print("rp_min:\t\t");Serial.print(rp_min);
  Serial.print("\trp_max:\t\t");Serial.println(rp_max);
}
