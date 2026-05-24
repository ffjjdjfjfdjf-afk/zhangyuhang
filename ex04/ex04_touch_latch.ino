// ex04: 触摸自锁开关（按一下翻转 LED 状态，软件防抖）
// 说明：根据你使用的触摸模块，调整 TOUCH_ACTIVE（HIGH 或 LOW）和引脚连接。

const int TOUCH_PIN = 2;     // 触摸传感器数字输出引脚
const int LED_PIN = 13;      // 指示 LED 引脚
const int TOUCH_ACTIVE = HIGH; // 触摸时传感器输出的电平（常见模块为 HIGH）

bool ledState = false;
int lastReading = LOW;
int lastStable = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // ms

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int reading = digitalRead(TOUCH_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == TOUCH_ACTIVE && lastStable != TOUCH_ACTIVE) {
      // 检测到从未触摸到触摸的上升沿，翻转 LED 状态
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
    lastStable = reading;
  }

  lastReading = reading;
}
