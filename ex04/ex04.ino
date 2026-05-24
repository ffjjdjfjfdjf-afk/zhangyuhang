const int touchPin = T0;
const int ledPin = 2;

const unsigned long debounceDelay = 50;  // 软件消抖时间

uint32_t touchBaseline = 0;
uint32_t touchThreshold = 0;

bool lastRawTouch = false;
bool stableTouch = false;
bool lastStableTouch = false;
unsigned long lastDebounceTime = 0;
bool ledState = LOW;

bool readTouchPressed() {
  int value = touchRead(touchPin);
  return value < (int)touchThreshold;
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

  Serial.printf("touch baseline=%u threshold=%u\n", touchBaseline, touchThreshold);
  Serial.println("ex04: 触摸自锁开关准备就绪");
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
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    Serial.printf("touch pressed -> led %s\n", ledState ? "ON" : "OFF");
  }

  lastStableTouch = stableTouch;
  delay(10);
}
