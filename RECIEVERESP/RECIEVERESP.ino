#include<ESP8266WiFi.h>
#include<espnow.h>

#define weapon D0

#define in1 D1
#define in2 D2
#define in3 D3
#define in4 D4

typedef struct data{
  bool weaponState; //button
  bool controllerMode; //switching
  int controllerVal;   //value
}data;

data myData;

void OnDataRecv(uint8_t* mac, uint8_t* incomingdata, uint8_t len){
  memcpy(&myData, incomingdata, sizeof(myData));

  if(myData.controllerMode == true){
    if(myData.controllerVal > 900 && myData.controllerVal <= 1024 ){
      forward();
    }
    else if(myData.controllerVal < 300 && myData.controllerVal > 0){
      back();
    }
    else if(myData.controllerVal < 800 && myData.controllerVal > 540){
      defaultState();
    }
    
    // Serial.println(myData.controllerVal);
  }
  else if(myData.controllerMode == false ){
    if(myData.controllerVal > 900 && myData.controllerVal <= 1024 ){
      left();
    }
    else if(myData.controllerVal < 300 && myData.controllerVal > 0){
      right();
    }
    else if(myData.controllerVal < 800 && myData.controllerVal > 540){
      defaultState();
    }
    // Serial.println(myData.controllerVal);
  }
  if(myData.weaponState == true){
    digitalWrite(weapon, HIGH);
    Serial.println("Weapon ON");
  }
  else{
    digitalWrite(weapon, LOW);
  }
  // Serial.println(myData.controllerMode);
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(weapon, OUTPUT);

  if(esp_now_init() != 0){
    Serial.println("Error initialising ESP NOW");
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){

}

void forward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Forward");
}

void back(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("back");
}

void left(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("left");
}

void right(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("right");
}

void defaultState(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("Rest");
}