#include <esp_now.h>
#include <WiFi.h>


// LED ချိတ်ဆက်ထားတဲ့ pin
const int ledPin = 2;

typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;

// Function ရဲ့ parameter ကိုအောက်ပါအတိုင်းပြင်လိုက်ပါ
void OnDataRecv(const esp_now_recv_info_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(myData.a);

  if (myData.a == 1) {
    digitalWrite(ledPin, HIGH); // LED ကိုဖွင့်မယ်
    Serial.println("LED ON");
  } else if (myData.a == 0) {
    digitalWrite(ledPin, LOW); // LED ကိုပိတ်မယ်
    Serial.println("LED OFF");
  }
}
 
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  // Wi-Fi subsystem ကိုစတင်ဖို့အတွက် WiFi.begin() ကိုခေါ်ရပါမယ်။
  WiFi.begin();
  Serial.print("Initializing WiFi...");
  while (WiFi.macAddress() == "00:00:00:00:00:00") {
    Serial.print(".");
    delay(100);
  }
  
  Serial.println(" Done!");
  // ESP32 ရဲ့ MAC address ကို Serial Monitor မှာကြည့်ရန်
  Serial.print("This board's MAC Address is: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // ဒီနေရာက function name မှာဘယ်လိုမှပြောင်းစရာမလိုပါဘူး။
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Receiver က data လက်ခံရုံသာမို့ loop ထဲမှာ ဘာမှမရှိပါဘူး။
}
