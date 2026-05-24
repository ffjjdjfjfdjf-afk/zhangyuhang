#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-EX07";
const char* ap_pass = "12345678";

const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

WebServer server(80);

void handleRoot() {
  const char* html = "<!doctype html>\n"
    "<html><head><meta charset=\"UTF-8\">\n"
    "<html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
    "<title>Web Dimmer</title>\n"
    "</head><body style=\"font-family:Arial;text-align:center;padding:20px\">\n"
    "<h2>Web 无极调光器</h2>\n"
    "<input id=\"s\" type=\"range\" min=\"0\" max=\"255\" value=\"0\" style=\"width:80%\">\n"
    "<div style=\"margin-top:12px\">亮度: <span id=\"v\">0</span></div>\n"
    "<script>\n"
    "let s=document.getElementById('s');let v=document.getElementById('v');\n"
    "let t=0;function send(val){fetch('/set?value='+val).catch(e=>console.log(e));}\n"
    "s.addEventListener('input',e=>{v.textContent=e.target.value;clearTimeout(t);t=setTimeout(()=>send(e.target.value),20);});\n"
    "</script>\n"
    "</body></html>\n";
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("value")) {
    int v = server.arg("value").toInt();
    if (v < 0) v = 0; if (v > 255) v = 255;
    ledcWrite(ledPin, v);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "missing value");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.print("AP "); Serial.println(ap_ssid);

  ledcAttach(ledPin, freq, resolution);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.begin();

  Serial.println("ex07 Web 无极调光器 已启动");
}

void loop() {
  server.handleClient();
}
