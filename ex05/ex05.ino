const int touchPin = T0;
const int ledPin = 2;

const unsigned long debounceDelay = 50;

uint32_t touchBaseline = 0;
uint32_t touchThreshold = 0;

bool lastRawTouch = false;
bool stableTouch = false;
bool lastStableTouch = false;
unsigned long lastDebounceTime = 0;

int speedLevel = 1;  // 1、2、3档
int dutyCycle = 0;
int fadeAmount = 1;

bool readTouchPressed() {
  int value = touchRead(touchPin);
  return value < (int)touchThreshold;
}

int getSpeedDelay() {
  switch (speedLevel) {
    case 1: return 20;
    case 2: return 12;
    case 3: return 6;
    default: return 12;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  delay(200);

  uint32_t total = 0;
  for (int i = 0; i < 32; i++) {
    total += touchRead(touchPin);
    delay(5);
  }

  touchBaseline = total / 32;
  touchThreshold = touchBaseline * 3 / 4;
  if (touchThreshold < 10) {
    touchThreshold = 10;
  }

  lastRawTouch = readTouchPressed();
  stableTouch = lastRawTouch;
  lastStableTouch = stableTouch;

  ledcAttach(ledPin, 5000, 8);

  Serial.printf("touch baseline=%u threshold=%u\n", touchBaseline, touchThreshold);
  Serial.println("ex05: 触摸调速呼吸灯准备就绪");
}

void loop() {
  bool currentRawTouch = readTouchPressed();

  if (currentRawTouch != lastRawTouch) {
    lastDebounceTime = millis();
    lastRawTouch = currentRawTouch;
  }

  if (millis() - lastDebounceTime > debounceDelay) {
    stableTouch = currentRawTouch;
  }

  if (stableTouch && !lastStableTouch) {
    speedLevel = speedLevel % 3 + 1;
    Serial.printf("touch pressed -> speed level %d\n", speedLevel);
  }

  lastStableTouch = stableTouch;

  ledcWrite(ledPin, dutyCycle);

  dutyCycle += fadeAmount;
  if (dutyCycle <= 0) {
    dutyCycle = 0;
    fadeAmount = 1;
  } else if (dutyCycle >= 255) {
    dutyCycle = 255;
    fadeAmount = -1;
  }

  delay(getSpeedDelay());
}
