#include <esp_now.h>
#include <WiFi.h>

// Receiver ရဲ့ MAC Address ထည့်ရန်
uint8_t receiverMAC[] = {0xF4, 0x65, 0x0B, 0xE5, 0x89, 0xB0};

// ပို့မယ့် data structure
typedef struct {
  int ledState; // LED ON = 1, OFF = 0
} Message;

Message myData;
esp_now_peer_info_t peerInfo;

// Callback: Data ပို့ပြီးသွားတဲ့အချိန်မှာ ခေါ်မယ့် function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Success" : "❌ Fail");
}

// Function: LED state ပို့မယ့် helper
void sendLedState(int state) {
  myData.ledState = state;
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.print("📤 Sent: LED ");
    Serial.println(state == 1 ? "ON (1)" : "OFF (0)");
  } else {
    Serial.println("❌ Error sending data");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
    return;
  }
}

void loop() {
  sendLedState(1); // LED ON
  delay(1000);

  sendLedState(0); // LED OFF
  delay(1000);
}
