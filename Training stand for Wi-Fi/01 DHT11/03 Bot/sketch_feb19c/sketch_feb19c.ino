// 
// Учебный стенд для занятий по Wi-Fi
//
// Определение температуры и влажности
// Результат выводится в Telegram-боте.
// 
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



// Библиотека датчика

#define PIN_SENSOR 0
// #define PIN_SENSOR 2 // только модуля расширения для ESP-01/ESP-01s

#include "DHT.h"
DHT dht(PIN_SENSOR, DHT11);




void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

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

  // Инициализация датчика

  dht.begin();

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
  if ( msg.text == "/display" ) commandDisplay(msg);

}



//
// "Поздороваться"
//

void commandSayHello(FB_msg& msg) {

  bot.sendMessage("Привет", msg.chatID);

}



//
// Вывести показания датчиков
//

void commandDisplay(FB_msg& msg) {

  String s = "";

  s += "<b>Показания датчиков</b>\n\n";

  s += "Tемпература: ";
  s += dht.readTemperature();
  s += " °C\n";
  
  s += "Влажность: ";
  s += dht.readHumidity();
  s += " %";

  bot.setTextMode(FB_HTML);
  bot.sendMessage(s, msg.chatID);

}