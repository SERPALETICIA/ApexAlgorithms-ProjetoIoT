//Nesta parte, as bibliotecas são incluídas.

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Define as credenciais da rede Wi-Fi (SSID e senha).

const char* ssid = "PROF.RAFAEL";
const char* password = "";

//A biblioteca WebServeré usada para criar um servidor web na porta 80 (padrão HTTP). O objeto serveré criado 
//para lidar com as proteções HTTP.

ESP8266WebServer server(80);
const int output5 = 5;//D1

//Define os parametros para configuração do IP Address.

IPAddress local_IP(192,168,1,147); 
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//A função handleRoot()gera o conteúdo HTML da página web que será servida quando o usuário acessar o endereço IP do 
//ESP8266 no navegador. A página contém botões para ligar e desligar um LED, bem como um controle deslizante simulando 
//um interruptor. Quando os botões são pressionados ou o controle deslizante é movido, ele envia as opções ao servidor 
//para ligar ou desligar o LED no pino 5 ( output5).

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
  html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";

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

//As funções handleOn()e handleOff()chamadas são quando o usuário clica nos botões "ON" e "OFF" na página web, 
//respectivamente. Eles alteraram o estado do pino de saída 5 ( output5) para ligar ou desligar o LED e enviaram uma 
//resposta de texto diminuindo o estado atual do LED.

void handleOn() {
  digitalWrite(output5, HIGH);
  server.send(200, "text/plain", "LED Ligado!");
}
void handleOff() {
  digitalWrite(output5, LOW);
  server.send(200, "text/plain", "LED Desligado!");
}

//A função setup()é chamada uma vez quando o dispositivo é inicializado. Ela configura o pino 5 como saída, 
//inicia a comunicação serial, configura a conexão Wi-Fi e define os manipuladores para as rotas do servidor web.

void setup() {
  Serial.begin(9600);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, LOW);
  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }

  //A função WiFi.begin(ssid, password)é usada para conectar o ESP8266 à rede Wi-Fi. O código aguarda até que a conexão 
  //seja estabelecida ( WiFi.status() == WL_CONNECTED) e imprima pontos no Serial Monitor durante a tentativa de conexão.

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

//A função loop()é chamada continuamente em um loop infinito. Ela trata as interações dos clientes do servidor web, 
//garantindo que o servidor web continue a funcionar enquanto aguarda as interações do usuário na página web.

void loop(){
server.handleClient();
}
