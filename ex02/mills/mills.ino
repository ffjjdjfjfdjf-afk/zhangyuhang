#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";  // 至少8位
const int LED_PIN = 2;

WebServer server(80);

void handleRoot() {
  String state = digitalRead(LED_PIN) ? "ON" : "OFF";
  String html = "<html><body style='text-align:center'>"
                "<h1>LED控制</h1><p>状态: <b>" + state + "</b></p>"
                "<a href='/on'><button>点亮</button></a> "
                "<a href='/off'><button>熄灭</button></a>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // AP模式：ESP32自己开热点
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("热点已开启");
  Serial.print("连接名称: ");
  Serial.println(ap_ssid);
  Serial.print("访问地址: http://");
  Serial.println(WiFi.softAPIP());  // 通常是 192.168.4.1

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}