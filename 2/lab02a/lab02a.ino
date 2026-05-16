// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("SOS Signal Starting...");
}

void loop() {
  // ========== S: 短闪3次 (200ms亮，200ms灭) ==========
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(300);  // 字母间隔

  // ========== O: 长闪3次 (600ms亮，200ms灭) ==========
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(600);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(300);  // 字母间隔

  // ========== S: 短闪3次 (200ms亮，200ms灭) ==========
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  
  delay(2000);  // 单词间隔（重复前等待2秒）
}