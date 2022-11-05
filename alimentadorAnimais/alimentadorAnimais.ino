//Programa: Modulo ESP32 Wifi com MQTT
//Autor: Arduino e Cia

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Gusthawo's Galaxy S22+";
const char* password = "uvcz4313";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

char mensagem[30];

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconectabroker();
  }


  //Envia a mensagem ao broker
  if (client.publish("/pac6iotalimentadoranimais1", "{ \"chatId\": 5538251965, \"type\": \"message\", \"content\": \"Opa, deu Boa o ChatBot!\" }")) {
    Serial.println("Mensagem enviada com sucesso...");
  }

  //Aguarda 30 segundos para enviar uma nova mensagem
  delay(10000);
  client.loop();
}

void reconectabroker() {
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32ClientPAC6", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao broker!");
    } else {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}