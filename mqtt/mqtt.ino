#include <WiFi.h>
#include <PubSubClient.h>
#define IO_USERNAME "leopinheiro132"
#define IO_KEY "aio_utCB38AekineHvr0NBNoIxqDRumL"      

const char* ssid = "";
const char* password = "********";
const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

#define LED 2

void setup_wifi() {
  delay(10);
  analogSetAttenuation(ADC_0db);           // Atenuação do ADC 1,1V - 4095
  pinMode(LED, OUTPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Messagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //messageTemp += (char)payload[i]; <----------Usar quando tiver uma mensagem na resposta do bloco
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED, HIGH);  // Turn the LED on (Note that LOW is the voltage level
      Serial.println("LED Ligado");
  } else {
    digitalWrite(LED, LOW);  // Turn the LED off by making the voltage HIGH
      Serial.println("LED Desligado");
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP32 - Sensores";
    clientId += String(random(0xffff), HEX);
    // Se conectado
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("conectado");
      // Depois de conectado, publique um anúncio ...
      client.publish("leopinheiro132/feeds/alimentadorfeed", "Iniciando Comunicação");
  //... e subscribe.
      client.subscribe("leopinheiro132/feeds/alimentadorfeed");
    }
     else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}
void setup() {
  pinMode(LED, OUTPUT);  // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttserver, 1883);  // Publicar
  client.setCallback(callback);        // Receber mensagem
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(5000);
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
/*
    pot = analogRead(POT) / 100;
    char s_pot[8];
    dtostrf(pot, 1, 2, s_pot);
    Serial.print("Potênciometro: ");
    Serial.println(s_pot);
    Serial.println(pot);
    client.publish("leduardocl/feeds/pot", s_pot);
    */
  }
}