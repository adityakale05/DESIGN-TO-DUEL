#include<ESP8266WiFi.h>
#include<espnow.h>

uint8_t macAddress[] = {0x08, 0x3A, 0x8D, 0xD3, 0x71, 0x49};

#define sw D5
#define vrX A0
#define weapon D1

typedef struct data{
  bool weaponState;
  bool controllerMode;
  int controllerVal;
}data;

data myData;

volatile bool modeC = false;  // false = Forward/Reverse, true = Left/Right
volatile bool modeW = false;
static unsigned long lastPressTime = 0;

IRAM_ATTR void  handleButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - lastPressTime > 500) {  // Debounce (200ms)
    modeC = !modeC;  // Toggle mode
    modeW = !modeW;
    lastPressTime = currentTime;

  }
}

void OnDataSent(uint8_t* macAddr, uint8_t sendstatus){
  Serial.println("Package status: ");
  if(sendstatus == 0){
    Serial.println("Package sent successfully");
  }
  else{
    Serial.println("Package failure");
  }
}

void setup(){
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);  
  pinMode(sw, INPUT_PULLUP);
  if(esp_now_init() != 0){
    Serial.println("ESP NOW not initialized");
    return;
  }

  attachInterrupt(digitalPinToInterrupt(sw), handleButtonPress, RISING);

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(macAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop(){
  myData.controllerVal = analogRead(vrX);
  myData.controllerMode = modeC;
  myData.weaponState = modeW;
  esp_now_send(macAddress, (uint8_t*)&myData, sizeof(myData));
  delay(1000);
}

