// this code is made by saksham Rajpal [ iOT India ]
// Youtube video : https://www.youtube.com/@iOT_India

#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Update to your slave's MAC

// Define structure for sending data
typedef struct struct_message {
  bool sw1;
  bool sw2;
  bool sw3;
  bool sw4;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == 0) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void manageSlave() {
  if (esp_now_add_peer(mac, WiFi.channel(), ESP_NOW_ROLE_SLAVE, NULL, 0) == 0) {
    Serial.println("Slave Added");
  } else {
    Serial.println("Slave Add Failed");
  }
}

void setup() {
  Serial.begin(115200);

  // Setup switches
  pinMode(D5, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: ");
  Serial.println(WiFi.macAddress());

  InitESPNow();
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  manageSlave();
}

void loop() {
  myData.sw1 = !digitalRead(D5); 
  myData.sw2 = !digitalRead(D1);
  myData.sw3 = !digitalRead(D7);
  myData.sw4 = !digitalRead(D4);

  esp_now_send(mac, (uint8_t *)&myData, sizeof(myData));
  delay(50);
}
