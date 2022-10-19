#include <ESP32Servo.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define TOKEN "5673829655:AAEkSRGqhVmKeH-DTlOLOIDz35ZtKbR6Aq0"
#define CHAT_ID "1577596807"

int servoPin = 13;
const char* ssid     = "CATOLICASC"; 
const char* password = "";

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

Servo myservo;
WiFiClientSecure client;
UniversalTelegramBot bot(TOKEN, client);

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
    }
  }
    
}

void connectToWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif

  while (WiFi.status() != WL_CONNECTED) {   
    Serial.print("Attemping to connect...");   
    delay(500);   
  }

  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial connected: 115200");
  delay(1500);

  // myservo.setPeriodHertz(50); 
  // myservo.attach(servoPin);
}
 
void loop() {
  // myservo.write(0); 
  // delay(2000);
  // myservo.write(180);
  // delay(2000);

  Serial.println("Teste");

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

}