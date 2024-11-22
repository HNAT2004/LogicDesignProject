#include <MFRC522.h>
#include <SoftwareSerial.h>

#define RST_PIN         4
#define SS_PIN          5
MFRC522 mfrc522(SS_PIN, RST_PIN);
uint8_t flag = 0;

  void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2, TX2
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
  Serial.println(uidStr);

  // Gửi request GET để kiểm tra UID
  if (0) // (1) {
    Serial2.write('1');
  } else {
    Serial2.write('2');
  }

  mfrc522.PICC_HaltA();
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



