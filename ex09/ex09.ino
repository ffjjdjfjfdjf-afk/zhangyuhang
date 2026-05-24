#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-EX09";
const char* ap_pass = "12345678";

const int touchPin = T0;

uint32_t touchBaseline = 0;
uint32_t touchThreshold = 0;

WebServer server(80);

bool readTouchRaw() {
  return touchRead(touchPin);
}

void handleRoot() {
  const char* html = "<!doctype html>\n"
    "<html><head><meta charset=\"UTF-8\">\n"
    "<html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
    "<title>实时传感器仪表盘</title>\n"
    "</head><body style=\"font-family:Arial;text-align:center;padding:20px\">\n"
    "<h2>触摸传感器实时数值</h2>\n"
    "<div style=\"font-size:48px;margin-top:30px\"><span id=\"val\">-</span></div>\n"
    "<script>function update(){fetch('/value').then(r=>r.text()).then(t=>{document.getElementById('val').textContent=t;});}setInterval(update,200);update();</script>\n"
    "</body></html>";
  server.send(200, "text/html", html);
}

void handleValue() {
  int v = touchRead(touchPin);
  server.send(200, "text/plain", String(v));
}

void setup() {
  Serial.begin(115200);
  delay(200);
  uint32_t total = 0;
  for (int i = 0; i < 32; i++) { total += touchRead(touchPin); delay(5); }
  touchBaseline = total / 32;
  touchThreshold = touchBaseline * 3 / 4;
  if (touchThreshold < 10) touchThreshold = 10;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/value", HTTP_GET, handleValue);
  server.begin();

  Serial.printf("touch baseline=%u threshold=%u\n", touchBaseline, touchThreshold);
  Serial.println("ex09 实时仪表盘 已启动");
}

void loop() {
  server.handleClient();
}
