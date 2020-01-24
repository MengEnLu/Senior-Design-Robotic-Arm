//Mega Serial 1 TX1 pin 18
//Mega Serial 1 RX1 pin 19


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino Mega slave ready to use");

  Serial1.begin(9600);  
  Serial.println("Arduino Mega serial 1 started at 9600");
  bluetoothSetUp();
  Serial.print("Ready");

  //test
  //String temp = "<111\t200\t\333\t\444\t\555\t666\t777\t888\t999\t000>";
  //Serial1.print(temp);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  String tempStr = "";
  String dataStr = "";
  bool dataReady = false;

  while(dataReady == false){
    tempStr = receive();
    //test
    //Serial.print("Incomplete: "); 
    //Serial.println(tempStr);
    if(findDataStr(tempStr) == true){
      dataStr = getDataStr(tempStr);
      dataReady = true;
    }
  }
  //test
  Serial.print("data String Ready: ");
  Serial.println(dataStr);
  
  //TO DO LIST:
  //INSERT FUNCTIONS TO EXTRACT DATA
  //INSERT FUNCTIONS FOR SERVOS AND MOTOR HERE

  //test delay is set to 1.5 sec
  delay(1500);
  clearBuffer();
  
}


//this function is used *ONLY* during SETUP
void bluetoothSetUp(){
  while(Serial1.available()){
    Serial.print(Serial1.read());
  }
}


String receive(){

  String str = "";
  bool flag = false;
  //char c;

  while(flag == false){
    String tempStr ="";
    if(Serial1.available()){
      tempStr = Serial1.readStringUntil('>');
    }

    int k = tempStr.length();
    if(k>=21 && k<=80){
      str = tempStr;
      flag = true;
    }
   
  }

  
  /*while(i<64){
    if(Serial1.available()){
      tempStr += Serial1.read();
      i++;
    }
  }
  */
  return str;
}

bool findDataStr(String tempStr){
  bool flag = false;
  for(int i = 0; i< tempStr.length(); i++){
    if(tempStr.charAt(i)== '<'){
      flag = true;
      break;
    }
  }
  return flag;
}

String getDataStr(String tempStr){
  int i = tempStr.indexOf('<');
  tempStr = tempStr.substring(i);
  int k = tempStr.indexOf('>');
  if (k <= tempStr.length()-1){
    tempStr = tempStr.substring(0,k+1);
  }
  else{
    tempStr = tempStr.substring(0,k);
    tempStr = tempStr + '>';
  }

  return tempStr;
}



void clearBuffer(){
  while(Serial1.available()){
    Serial1.read();
  }
}
