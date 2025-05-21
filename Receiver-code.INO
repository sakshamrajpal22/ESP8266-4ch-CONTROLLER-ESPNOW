// this code is made by saksham Rajpal [ iOT India ]
// Youtube video : https://www.youtube.com/@iOT_India

#include <ESP8266WiFi.h>
#include <espnow.h>

#define CHANNEL 0

#define LED1 D1
#define LED2 D2
#define LED3 D3
#define LED4 D4

typedef struct struct_message {
  bool sw1;
  bool sw2;
  bool sw3;
  bool sw4;
} struct_message;

struct_message incomingData;

unsigned long lastRecvTime = 0;
const unsigned long timeout = 500; // 500ms timeout

void OnDataRecv(uint8_t *mac, uint8_t *incomingDataRaw, uint8_t len) {
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  Serial.print("Switch states: ");
  Serial.print(incomingData.sw1); Serial.print(" ");
  Serial.print(incomingData.sw2); Serial.print(" ");
  Serial.print(incomingData.sw3); Serial.print(" ");
  Serial.println(incomingData.sw4);

  digitalWrite(LED1, incomingData.sw1);
  digitalWrite(LED2, incomingData.sw2);
  digitalWrite(LED3, incomingData.sw3);
  digitalWrite(LED4, incomingData.sw4);

  lastRecvTime = millis(); // Update last received time
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Slave_1", "Slave_1_Password", CHANNEL, 0);

  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());

  if (esp_now_init() == 0) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // If no data received recently, turn off LEDs
  if (millis() - lastRecvTime > timeout) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }
}
