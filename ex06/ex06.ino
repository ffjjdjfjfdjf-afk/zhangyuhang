const int ledPinA = 2;
const int ledPinB = 15;

const int freq = 5000;
const int resolution = 8;

int dutyCycle = 0;
int direction = 1;

void setup() {
  Serial.begin(115200);

  ledcAttach(ledPinA, freq, resolution);
  ledcAttach(ledPinB, freq, resolution);

  Serial.println("ex06: 双通道警车双闪准备就绪");
}

void loop() {
  ledcWrite(ledPinA, dutyCycle);
  ledcWrite(ledPinB, 255 - dutyCycle);

  dutyCycle += direction;
  if (dutyCycle >= 255) {
    dutyCycle = 255;
    direction = -1;
  } else if (dutyCycle <= 0) {
    dutyCycle = 0;
    direction = 1;
  }

  delay(10);
}
