// 
// Учебный стенд для занятий по Wi-Fi
//
// Управление реле через Интернет
// Версия для Telegram-бота
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека BOT

#include <FastBot.h>

// Ссылка, которая позволит вам создать бота - https://t.me/BotFather

FastBot bot("" /* BOT_TOKEN */);

// Pin нагрузки

#define PIN_RELAY 0




void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  // digitalWrite(PIN_RELAY, HIGH); // только модуля расширения для ESP-01/ESP-01s

  Serial.begin(9600);

  Serial.println();
  Serial.println("Hello");

  // Инициализация Wi-Fi

  WiFi.begin(MY_SSID, MY_PASS);

  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Инициализация бота
  
  bot.attach(newMsg);

}



void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);
  
  // Помощник бота

  tick();

}


void tick() { bot.tick(); }



// 
// Обработчика сообщений бота
//

void newMsg(FB_msg& msg) {

  Serial.print(msg.userID);
  Serial.print(", ");
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.println(msg.text); 

  if ( msg.text == "/say_hello" ) commandSayHello(msg);
  if ( msg.text == "/on" ) commandOn(msg);
  if ( msg.text == "/off" ) commandOff(msg);

}



//
// "Поздороваться"
//

void commandSayHello(FB_msg& msg) {

  bot.sendMessage("Привет", msg.chatID);

}



//
// Включить
//

void commandOn(FB_msg& msg) {

  digitalWrite(PIN_RELAY, HIGH);
  // digitalWrite(PIN_RELAY, LOW); // только модуля расширения для ESP-01/ESP-01s
  bot.sendMessage("On", msg.chatID);

}



//
// Выключить
//

void commandOff(FB_msg& msg) {

  digitalWrite(PIN_RELAY, LOW);
  // digitalWrite(PIN_RELAY, HIGH); // только модуля расширения для ESP-01/ESP-01s
  bot.sendMessage("Off", msg.chatID);

}
