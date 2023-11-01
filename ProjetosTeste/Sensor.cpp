//Nesta parte, as bibliotecas são incluídas.

#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//Define as credenciais da rede Wi-Fi (SSID e senha).

const char* ssid = "Gabriel's Galaxy S21+";
const char* password = "12345678";

//configuração dos pino digital ( D1) ao qual o sensor DHT11 está conectado. DHT dht(DHTPIN, DHTTYPE);cria um objeto do
// tipo DHT para leitura dos dados do sensor.
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//A biblioteca WebServeré usada para criar um servidor web na porta 80 (padrão HTTP). O objeto serveré criado 
//para lidar com as proteções HTTP.

WiFiServer server(80);

void setup() {
  //Define os parametros para configuração do IP Address.

  IPAddress local_IP(192,168,85,194); 
  IPAddress gateway(192,168,85,114);
  IPAddress subnet(255,255,255,0);

  //inicia a comunicação serial para depuração.

  Serial.begin(9600);

  //inicializa o sensor DHT11.

  dht.begin();

  //Esta linha imprime uma linha em branco no Serial Monitor

  Serial.println();

 //Esta linha imprime a string "Conectando-se a " no Serial Monitor, sem pular para uma nova linha.

  Serial.print("Conectando-se a ");

  // imprime o nome da rede Wi-Fi (SSID) ao qual o NodeMCU está tentando se conectar. 

  Serial.println(ssid);

  //configura o endereço IP estático.

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  //Conecta à rede Wi-Fi usando as credenciais fornecidas.

  WiFi.begin(ssid, password);

  // Aguarda até que o NodeMCU esteja conectado à rede Wi-Fi.

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  
  }
  //Imprime uma linha em branco no Serial Monitor.

  Serial.println("");

  // Esta linha imprime a mensagem "Conexão WiFi estabelecida" no Serial Monitor. Isso indica que o NodeMCU conseguiu se 
  //conectar à rede Wi-Fi com sucesso.

  Serial.println("Conexão WiFi estabelecida");

  //Esta linha imprime a string "Endereço IP: " no Serial Monitor, sem pular para uma nova linha. 
  Serial.print("Endereço IP: ");

  // imprime o endereço IP local atribuído ao NodeMCU no Serial Monito

  Serial.println(WiFi.localIP());

  // Inicia o servidor web no NodeMCU na porta 80

  server.begin();
}

// A função loop()é chamada repetidamente. Ela verifica se há clientes esperando se conectar ao servidor web. 
//Se não houver cliente, a função retorna. Se houver um cliente disponível, o código dentro do bloco é executado para 
//lidar com as configurações HTTP.

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  //le a solicitação HTTP do cliente.

  String request = client.readStringUntil('\r');

  //limpa o buffer de entrada.

  client.flush();

  //lêem a umidade e temperatura do sensor DHT11, respectivamente.

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Umidade: "); Serial.println(h);
  Serial.print("Temperatura: "); Serial.println(t);

  //Definindo parametros pagina HTML
  
  String html = R"====(
  <html>
  <head>
  <link href="https://fonts.googleapis.com/css2?family=Anton&display=swap"
  rel="stylesheet">
  <link rel="stylesheet"
  href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
  <style>
  body {
  font-family: 'Anton';
  background: linear-gradient(to right, #83a4d4, #b6fbff);
  color: #333;
  height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  }
  h2 {
  font-size: 2.5em;
  }
  p {
  font-size: 2em;
  }
  fas {
    ont-size: 1.5em;
  margin-right: 10px;
  }
  </style>
  <script>
  setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
  var readings = this.responseText.split(";");
  document.getElementById("humidity").innerHTML = readings[0] + " %";
  document.getElementById("temperature").innerHTML = readings[1] + "&deg;C";
  }
  };
  xhttp.open("GET", "/get-data", true);
  xhttp.send();
  }, 2000); // Atualiza a cada 2 segundos
  </script>
  </head>
  <body>
  <h2>NodeMCU com DHT11</h2>
  <h3>Prof. Rafael Barbosa</h3>
  <p><i class="fas fa-tint"></i> Umidade: <span id="humidity"></span></p>
  <p><i class="fas fa-thermometer-half"></i> Temperatura: <span
  id="temperature"></span></p>
  </body>
  </html>
  )====";

  // Verificar se a solicitação é para "/get-data"

  if (request.indexOf("/get-data") != -1) {
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    client.print(String(h) + ";" + String(t));
    return;
  }

  // Se não for para "/get-data", servir a página HTML

  else {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.print(html);
  }
  delay(1);
  client.stop();
}
