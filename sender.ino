#include <esp_now.h>
#include <WiFi.h>

// Receiver ဘုတ်ရဲ့ MAC address ကိုဒီနေရာမှာထည့်ပေးပါ။
uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0xE5, 0x89, 0xB0};

typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // LED ကိုဖွင့်ဖို့ '1' ကိုပို့မယ်
  myData.a = 1;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent '1' (Turn LED ON)");
  }
  else {
    Serial.println("Error sending '1'");
  }
  delay(1000); // 1 စက္ကန့် စောင့်မယ်

  // LED ကိုပိတ်ဖို့ '0' ကိုပို့မယ်
  myData.a = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent '0' (Turn LED OFF)");
  }
  else {
    Serial.println("Error sending '0'");
  }
  delay(1000); // 1 စက္ကန့် စောင့်မယ်
}
