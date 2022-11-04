//Programa: Modulo ESP32 Wifi com MQTT
//Autor: Arduino e Cia

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WickedBotz";
const char* password =  "wickedbotz";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

int contador = 1;
char mensagem[30];

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
}

void loop()
{
  reconectabroker();
  Serial.print("Estado pré envio da mensagem: ");
  Serial.println(client.connected());
  //Faz a conexao com o broker MQTT
  sprintf(mensagem, "MQTT ESP32 - Mensagem no. %d", contador);
  Serial.print("Mensagem enviada: ");
  Serial.println(mensagem);


  //Envia a mensagem ao broker
  client.publish("/pac6iotalimentadoranimais1", "{ 'chatId': 5538251965, 'type': 'message', 'content': 'Opa, deu Boa o ChatBot!' }");
  Serial.println("Mensagem enviada com sucesso...");
  
  //Incrementa o contador
  contador++;
  
  //Aguarda 30 segundos para enviar uma nova mensagem
  delay(10000);
}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}