#include <WiFi.h>
#include <stdio.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password =  "";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

char* topico="/pac6iotalimentadoranimais1";
char* mensagem = "{ "
                 " \"chatId\": 5538251965,"
                 " \"type\": \"message\","
                 " \"content\": \"Alimento Liberado com Sucesso!\""
                 "}";

Servo myservo;
int servoPin = 13;

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
  myservo.setPeriodHertz(50); 
  myservo.attach(servoPin);

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

  liberarComida();

  if (client.publish(topico, mensagem)) {
    Serial.println("Mensagem enviada com sucesso...");
  }

  delay(5000);  
  client.loop();
}

void liberarComida() {
  myservo.write(90);
  delay(1000);
  myservo.write(0); 
}

void reconectabroker() {
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