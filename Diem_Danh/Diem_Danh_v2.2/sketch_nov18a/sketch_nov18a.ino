#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <HardwareSerial.h>

const char* ssid = "ACLAB";         // Tên mạng WiFi
const char* password = "ACLAB2023";  // Mật khẩu WiFi
const char* serverIP = "192.168.238.152"; // Địa chỉ IP của máy tính
const int serverPort = 8080;            // Cổng của web server

#define RST_PIN         4
#define SS_PIN          5

MFRC522 mfrc522(SS_PIN, RST_PIN);
HardwareSerial mySerial(2); // UART2

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 16, 17); // RX2, TX2
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
        String ID = response;
        ID += "#";
        for (int i = 0; ID[i] != '\0'; i++) {
          uint8_t value = (uint8_t)ID[i];
          mySerial.write(value);
        }
        Serial.println(response); // In dữ liệu ra Serial Monitor
        
      } else {
        String data = "ERROR";
        for (int i = 0; data[i] != '\0'; i++) {
          uint8_t value = (uint8_t)data[i];
          mySerial.write(value);
          receiveAndPrintData(); // Gọi hàm nhận dữ liệu ngay sau khi gửi
        }
        Serial.println("ERROR");
      }

      http.end();
      mfrc522.PICC_HaltA();
    }
  }

  // Gọi hàm nhận dữ liệu từ RX và in ra màn hình liên tục
  receiveAndPrintData();

  delay(3000); // Giảm thời gian chờ giữa các lần quét thẻ
}

void receiveAndPrintData() {
  while (mySerial.available()) {
    char receivedChar = mySerial.read();
    Serial.print("Received: ");
    Serial.println(receivedChar);
  }
}