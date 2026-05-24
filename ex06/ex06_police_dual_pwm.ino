// ex06: 警车双闪灯效（双通道 PWM，反相信号）
// 两个 LED 的亮度呈反相关系：一个上升时另一个下降

const int LED_A = 9;  // PWM 引脚 A
const int LED_B = 10; // PWM 引脚 B

int brightness = 0;
int direction = 1; // 1: 递增, -1: 递减
const int step = 1; // 每次亮度变化步进
unsigned long previousMillis = 0;
const unsigned long interval = 5; // 控制渐变速度，数值越小越快

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  analogWrite(LED_A, 0);
  analogWrite(LED_B, 255);
}

void loop() {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    brightness += direction * step;
    if (brightness <= 0) {
      brightness = 0;
      direction = 1;
    } else if (brightness >= 255) {
      brightness = 255;
      direction = -1;
    }

    analogWrite(LED_A, brightness);
    analogWrite(LED_B, 255 - brightness);
  }
}
