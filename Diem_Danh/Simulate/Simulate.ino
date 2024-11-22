#include <MFRC522.h>
#define RX2_PIN 16 // RX2 - GPIO16
#define TX2_PIN 17 // TX2 - GPIO17
#define RST_PIN         4
#define SS_PIN          5

MFRC522 mfrc522(SS_PIN, RST_PIN);
String receivedData0 = "";
String receivedData2 = "";
int flag1 = 0;
int flag2 = 0;
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);  // Cấu hình UART2 với RX và TX
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // Đọc UID và chuyển đổi thành chuỗi
      String uidStr = "";
     for (byte i = 0; i < mfrc522.uid.size; i++) {
        uidStr += String(mfrc522.uid.uidByte[i], HEX);
      }
      // Chuyển đổi String thành mảng byte
      const char* uidCharArray = uidStr.c_str();  // Lấy con trỏ tới mảng byte của String

      // Gửi chuỗi UID qua UART (TX0)
      Serial.write((const uint8_t*)uidCharArray, uidStr.length()); 
      flag1 = 1;
      flag2 = 1;
      mfrc522.PICC_HaltA();
    }
  // Kiểm tra dữ liệu nhận từ UART2
   if(flag1 == 1){
  while(Serial2.available()) {
    char receivedChar = Serial2.read();  // Đọc ký tự nhận được
    receivedData2 += receivedChar;
  }
 
  if (receivedData2.length() > 0) {
     Serial2.println(receivedData2); 
     Serial2.write("2213707"); 
     receivedData2 = ""; // Xóa biến chuỗi sau khi in 
     }
     flag1 = 0;
  }
   if(flag2 == 1){
  while(Serial.available()) {
    char receivedChar = Serial.read();  // Đọc ký tự nhận được
    receivedData0 += receivedChar;
  }
  if (receivedData0.length() > 0) {
     Serial.println(receivedData0); 
     receivedData0 = ""; // Xóa biến chuỗi sau khi in 
     }
    flag2 = 0;
  }
  delay(2000);
}