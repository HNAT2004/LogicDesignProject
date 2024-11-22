#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include "time.h" // Sử dụng thư viện time.h

const char* ssid = "ACLAB";         // Tên mạng WiFi
const char* password = "ACLAB2023";  // Mật khẩu WiFi
const char* serverIP = "10.28.128.148"; // Địa chỉ IP của máy tính
const int serverPort = 8080;            // Cổng của web server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;    // GMT+7
const int daylightOffset_sec = 0;

#define RST_PIN         4
#define SS_PIN          5
MFRC522 mfrc522(SS_PIN, RST_PIN);
uint8_t flag = 0;

  void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2, TX2
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  Serial.println("WiFi connected");

  // Init and get the time from NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
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
        Serial.println(response);
        if(response[0] == 'I'){
          flag = 2;
        }
        else if(response[0] != 'I'){
        flag = 1;
        }
         // In dữ liệu ra Serial Monitor

      } else {
        Serial.println("ERROR");
      }

      http.end();
      mfrc522.PICC_HaltA();
    }
  }
if (flag == 1) {
    Serial.println("1");
    Serial2.write('1'); // Gửi ký tự '1'
    flag = 0;
} else if (flag == 2) {
    Serial.println("0");
    Serial2.write('0'); // Gửi ký tự '0'
    flag = 0;
}

  // Gọi hàm nhận dữ liệu từ RX và in ra màn hình liên tục
 
  delay(3000); // Giảm thời gian chờ giữa các lần quét thẻ
}


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.printf("%02d:%02d:%02d %02d/%02d/%04d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
}
