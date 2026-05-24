// ex05: 多档位触摸调速呼吸灯
// 触摸一次切换档位（1/2/3），呼吸速度随档位变化

const int TOUCH_PIN = 2;     // 触摸传感器引脚
const int LED_PIN = 9;       // PWM 引脚，用于呼吸灯
const int TOUCH_ACTIVE = HIGH;

int speedLevel = 0; // 0 = 慢, 1 = 中, 2 = 快
int lastReading = LOW;
int lastStable = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

int brightness = 0;
int fadeAmount = 1; // 每次改变的亮度步进
unsigned long previousMillis = 0;
unsigned long interval = 20; // 动态由档位决定

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  // 读取触摸并做软件防抖，上升沿触发档位切换
  int reading = digitalRead(TOUCH_PIN);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == TOUCH_ACTIVE && lastStable != TOUCH_ACTIVE) {
      speedLevel = (speedLevel + 1) % 3;
    }
    lastStable = reading;
  }
  lastReading = reading;

  // 根据档位设置更新时间间隔（间隔越大呼吸越慢）
  if (speedLevel == 0) interval = 30; // 慢
  else if (speedLevel == 1) interval = 12; // 中
  else interval = 5; // 快

  // 非阻塞式渐变实现
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
      // 保持边界值在 0..255
      brightness = constrain(brightness, 0, 255);
    }
    analogWrite(LED_PIN, brightness);
  }
}
