#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-EX08";
const char* ap_pass = "12345678";

const int touchPin = T0;
const int ledPin = 2;
const unsigned long debounceDelay = 50;

uint32_t touchBaseline = 0;
uint32_t touchThreshold = 0;

bool lastRawTouch = false;
bool stableTouch = false;
bool lastStableTouch = false;
unsigned long lastDebounceTime = 0;

bool armed = false;
bool alarmState = false;

WebServer server(80);

bool readTouchPressed() {
  int v = touchRead(touchPin);
  return v < (int)touchThreshold;
}

void handleRoot() {
  const char* html = "<!doctype html>\n"
   "<html><head><meta charset=\"UTF-8\">\n"
    "<html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
    "<title>安防主机</title>\n"
    "</head><body style=\"font-family:Arial;text-align:center;padding:20px\">\n"
    "<h2>物联网安防模拟</h2>\n"
    "<button onclick=\"fetch('/arm')\">布防(Arm)</button>\n"
    "<button onclick=\"fetch('/disarm')\">撤防(Disarm)</button>\n"
    "<div style=\"margin-top:12px\">状态: <span id=\"s\">未知</span></div>\n"
    "<script>function update(){fetch('/status').then(r=>r.json()).then(j=>{document.getElementById('s').textContent='armed:'+j.armed+' alarm:'+j.alarm;});}setInterval(update,500);update();</script>\n"
    "</body></html>";
  server.send(200, "text/html", html);
}

void handleArm() {
  armed = true;
  server.send(200, "text/plain", "ARMED");
}

void handleDisarm() {
  armed = false;
  alarmState = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "DISARMED");
}

void handleStatus() {
  String j = "{";
  j += "\"armed\":"; j += (armed?"true":"false"); j += ",";
  j += "\"alarm\":"; j += (alarmState?"true":"false");
  j += "}";
  server.send(200, "application/json", j);
}

unsigned long lastFlash = 0;
bool flashOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  delay(200);
  uint32_t total = 0;
  for (int i = 0; i < 32; i++) { total += touchRead(touchPin); delay(5); }
  touchBaseline = total / 32;
  touchThreshold = touchBaseline * 3 / 4;
  if (touchThreshold < 10) touchThreshold = 10;

  lastRawTouch = readTouchPressed();
  stableTouch = lastRawTouch;
  lastStableTouch = stableTouch;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/arm", HTTP_GET, handleArm);
  server.on("/disarm", HTTP_GET, handleDisarm);
  server.on("/status", HTTP_GET, handleStatus);
  server.begin();

  Serial.println("ex08 安防主机 已启动");
}

void loop() {
  server.handleClient();

  bool currentRawTouch = readTouchPressed();
  if (currentRawTouch != lastRawTouch) { lastDebounceTime = millis(); lastRawTouch = currentRawTouch; }
  if (millis() - lastDebounceTime > debounceDelay) stableTouch = currentRawTouch;
  if (stableTouch && !lastStableTouch) {
    if (armed) {
      alarmState = true; // 触发后锁定报警，直到网页撤防
      Serial.println("Alarm triggered!");
    }
  }
  lastStableTouch = stableTouch;

  if (alarmState) {
    unsigned long now = millis();
    if (now - lastFlash >= 100) { lastFlash = now; flashOn = !flashOn; digitalWrite(ledPin, flashOn ? HIGH : LOW); }
  }
}
