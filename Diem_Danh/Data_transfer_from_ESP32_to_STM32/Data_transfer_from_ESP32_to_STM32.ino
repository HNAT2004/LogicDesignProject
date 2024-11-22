void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 Serial2.begin(115200, SERIAL_8N1, 16, 17);
}
String receivedData = "";
void loop() {
  // put your main code here, to run repeatedly:
      while (Serial.available()) { 
      char receivedChar = Serial.read(); 
      Serial2.write(receivedChar);
    }
  receivedData = "";
  delay(3000);
}
