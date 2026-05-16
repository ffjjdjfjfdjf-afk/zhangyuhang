onst int ledPin = 2;

// SOS 闪烁序列（单位：毫秒）
// 正数=亮，负数=灭，0=序列结束
const int sosPattern[] = {200, -200, 200, -200, 200, -300,   // S: 短闪3次
                          600, -200, 600, -200, 600, -300,   // O: 长闪3次
                          200, -200, 200, -200, 200, -2000}; // S: 短闪3次 + 结束停顿
const int patternLength = sizeof(sosPattern) / sizeof(sosPattern[0]);

int patternIndex = 0;           // 当前执行到序列的第几步
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("SOS Non-blocking Starting...");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 检查当前步骤是否完成
  int stepDuration = sosPattern[patternIndex];
  if (stepDuration < 0) stepDuration = -stepDuration;  // 取绝对值
  
  if (currentMillis - previousMillis >= (unsigned long)stepDuration) {
    previousMillis = currentMillis;
    
    // 移动到下一步
    patternIndex++;
    if (patternIndex >= patternLength) {
      patternIndex = 0;  // 序列结束，从头开始
    }
    
    // 设置LED状态（正数亮，负数灭）
    ledState = (sosPattern[patternIndex] > 0) ? HIGH : LOW;
    digitalWrite(ledPin, ledState);
  }
}