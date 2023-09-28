#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "PROF.RAFAEL";
const char* password = "";

ESP8266WebServer server(80);
const int output5 = 5;//D1

IPAddress local_IP(192,168,13,247); //Troque o IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void handleRoot();
void handleOn();
void handleOff();
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #77878A;}</style></head>";
  html = "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #77878A;}";

  html += ".switch {";
  html += "  position: relative;";
  html += "  display: inline-block;";
  html += "  width: 60px;";
  html += "  height: 34px;";
  html += "}";
  html += "/* Hide default HTML checkbox */";
  html += ".switch input {";
  html += "  opacity: 0;";
  html += "  width: 0;";
  html += "  height: 0;";
  html += "}";
  html += "/* The slider */";
  html += ".slider {";
  html += "  position: absolute;";
  html += "  cursor: pointer;";
  html += "  top: 0;";
  html += "  left: 0;";
  html += "  right: 0;";
  html += "  bottom: 0;";
  html += "  background-color: #ccc;";
  html += "  -webkit-transition: .4s;";
  html += "  transition: .4s;";
  html += "}";
  html += ".slider:before {";
  html += "  position: absolute;";
  html += "  content: \"\";";
  html += "  height: 26px;";
  html += "  width: 26px;";
  html += "  left: 4px;";
  html += "  bottom: 4px;";
  html += "  background-color: white;";
  html += "  -webkit-transition: .4s;";
  html += "  transition: .4s;";
  html += "}";
  html += "input:checked + .slider {";
  html += "  background-color: #2196F3;";
  html += "}";
  html += "input:focus + .slider {";
  html += "  box-shadow: 0 0 1px #2196F3;";
  html += "}";
  html += "input:checked + .slider:before {";
  html += "  -webkit-transform: translateX(26px);";
  html += "  -ms-transform: translateX(26px);";
  html += "  transform: translateX(26px);";
  html += "}";
  html += "/* Rounded sliders */";
  html += ".slider.round {";
  html += "  border-radius: 34px;";
  html += "}";
  html += ".slider.round:before {";
  html += "  border-radius: 50%;";
  html += "}";
  html += "</style></head>";


  html += "<body><h1>ESP8266 Web Server</h1>";
  html += "<h2>Prof. Rafael</h2>";
  // html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  // html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";

  html += "<label class=\"switch\">";
  html += "<input type=\"checkbox\">";
  html += "<span class=\"slider round\"></span>";
  html += "</label>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += " var xhr = new XMLHttpRequest();";
  html += " xhr.open('GET', '/5/' + state, true);";
  html += " xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(output5, HIGH);
  server.send(200, "text/plain", "LED Ligado!");
}
void handleOff() {
  digitalWrite(output5, LOW);
  server.send(200, "text/plain", "LED Desligado!");
}
void setup() {
  Serial.begin(9600);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, LOW);
  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/5/on", HTTP_GET, handleOn);
  server.on("/5/off", HTTP_GET, handleOff);
  server.begin();
}
void loop(){
server.handleClient();
}