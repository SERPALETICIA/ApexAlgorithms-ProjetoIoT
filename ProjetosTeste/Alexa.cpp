    //Nesta parte, as bibliotecas são incluídas.

    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <SinricPro.h>
    #include <SinricProSwitch.h>

    // Essas são definições de constantes. WIFI_SSID e WIFI_PASS são as credenciais de sua rede Wi-Fi. APP_KEY e APP_
    //SECRET são chaves de autenticação para a plataforma SinricPro. SWITCH_ID é o identificador exclusivo para o 
    //dispositivo do tipo interruptor na plataforma SinricPro.

    #define WIFI_SSID "PROF.RAFAEL"
    #define WIFI_PASS ""
    #define APP_KEY "bfb745fc-7562-4bd7-a502-34189c2eff4c"
    #define APP_SECRET "a4b3f8d1-1a2b-4595-b4d8-0e3b5c0c9265-d2483900-1a51-47a0-a2fc-d621a3cf9afc"
    #define SWITCH_ID "651dfc9d9216999cea397160"
    #define BAUD_RATE 9600

    //Esses são os pinos GPIO do ESP8266 usados ​​para o botão físico (BUTTON_PIN) e o relé (RELE_PIN).

    #define BUTTON_PIN 0 
    #define RELE_PIN 5

    //myPowerState armazena o estado atual do dispositivo (ligado ou desligado). lastBtnPress guarda o valor do tempo 
    //(em milissegundos) quando o botão foi pressionado pela última vez, utilizado para evitar rebotes (rejeições).
    bool myPowerState = false; 
    unsigned long lastBtnPress = 0; 

    //Esta função é chamada sempre que o estado do dispositivo é alterado remotamente pela plataforma SinricPro. Ela 
    //imprime informações no Serial Monitor e atualiza o estado do dispositivo de acordo com o estado recebido.
    bool onPowerState(const String &deviceId, bool &state) {
    
    Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state?"on":"off");
    myPowerState = state;
   
    digitalWrite(RELE_PIN, myPowerState?LOW:HIGH);
    return true;
    }
    
    //Esta função é chamada no loop principal e verifica se o botão físico foi pressionado. Se sim, ela muda o estado do 
    //dispositivo e envia essa mudança de estado para a plataforma SinricPro.

    void handleButtonPress() {
    unsigned long actualMillis = millis();

    if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000) {
    myPowerState = !myPowerState;
    digitalWrite(RELE_PIN, myPowerState?LOW:HIGH);
    
    SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
    mySwitch.sendPowerStateEvent(myPowerState);
    Serial.printf("Device %s turned %s (manually via flashbutton)\r\n",
    mySwitch.getDeviceId().c_str(), myPowerState?"on":"off");
    lastBtnPress = actualMillis;
    }
    }
    
    //Esta função tenta se conectar à rede Wi-Fi usando as credenciais detectadas e imprime o endereço IP do ESP8266 no 
    //Serial Monitor quando a conexão for bem sucedida.
    void setupWiFi() {
        Serial.printf("\r\n[Wifi]: Connecting");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
    }
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
    }
    
    //Esta função configura o objeto SinricProSwitch associado ao SWITCH_ID. Defina a função de callback onPowerState()
    // e outras funções de callback para lidar com eventos de conexão/desconexão à plataforma SinricPro.
    void setupSinricPro() {
    SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
    mySwitch.onPowerState(onPowerState);
    
    //configuram callbacks para lidar com eventos de conexão e desconexão com a plataforma SinricPro. Quando o dispositivo
    // é conectado ou desconectado com sucesso, mensagens informativas são impressas no Serial Monitor para ajudar no processo de depuração e monitoramento do status da conexão com a plataforma.
    SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
    SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
    SinricPro.begin(APP_KEY, APP_SECRET);
    }
    
    //Esta função é chamada uma vez quando o dispositivo é ligado. Ela configura os pinos como entrada/saída, inicia a 
    //comunicação serial, conecta o ESP8266 ao Wi-Fi e configura a conexão com SinricPro

    void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RELE_PIN, OUTPUT);
    digitalWrite(RELE_PIN, HIGH);
    Serial.begin(BAUD_RATE);
    setupWiFi();
    setupSinricPro();
    }

    //Esta função é chamada repetidamente em um loop infinito. Ela lida com pressão do botão físico ativa a função 
    //handleButtonPress() e também verifica e trata eventos da plataforma SinricPro usando SinricPro.handle().

    void loop() {
    handleButtonPress();
    SinricPro.handle(); 
    }