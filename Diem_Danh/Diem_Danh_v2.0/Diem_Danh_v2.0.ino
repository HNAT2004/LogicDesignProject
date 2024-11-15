#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
const char* ssid = "ACLAB";         // Tên mạng WiFi
const char* password = "ACLAB2023";  // Mật khẩu WiFi
const char* serverIP = "10.28.128.148"; // Địa chỉ IP của máy tính
const int serverPort = 8080;            // Cổng của web server

#define RST_PIN         4
#define SS_PIN          5

MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial mySerial(16, 17); // RX, TX 
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  int flag = 0;
  if(flag == 0){
  if (WiFi.status() == WL_CONNECTED) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // Đọc UID và chuyển đổi thành chuỗi
      String uidStr = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uidStr += String(mfrc522.uid.uidByte[i], HEX);
      }

      HTTPClient http;
      String serverUrl = "http://" + String(serverIP) + ":" + String(serverPort) + "/check_uid?uid=" + uidStr;
      http.begin(serverUrl);

      // Gửi request GET để kiểm tra UID
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String response = http.getString(); // Lấy response từ server
        flag = 1;
        Serial.println(response); // In dữ liệu ra Serial Monitor
      } else {
       String data = "ERROR";
        for (int i = 0; data[i] != '\0'; i++) {
          uint8_t value = (uint8_t)data[i];
          mySerial.write(value);
        }
        Serial.println("ERROR");
      }

      http.end();
      mfrc522.PICC_HaltA();
    }
  }
  }
  delay(1000);
  if(flag == 1){
  const char* data = "1";
        for (int i = 0; data[i] != '\0'; i++) {
          uint8_t value = (uint8_t)data[i];
          mySerial.write(value);
        }
        flag = 0;
      
  }
  delay(4000); // Giảm thời gian chờ giữa các lần quét thẻ
}
