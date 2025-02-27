// 
// Учебный стенд для занятий по Wi-Fi
//
// Определение температуры и влажности
// Результат выводится на консоль Telnet
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека Telnet

#include <TLog.h>
#include <TelnetSerialStream.h>
TelnetSerialStream telnetSerialStream = TelnetSerialStream();

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
  
  // Инициализация Telnet

  Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
  Log.begin();

  // Инициализация датчика

  dht.begin();

}



void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);
  
  // Помощник веб-сервера

  Log.loop();

  // Отправка в консоль Telnet

  static uint32_t timer = 0;

  if ( millis() - timer > 10000 ) {
  
    timer = millis();
  
    Log.print("t:");
    Log.println(dht.readTemperature());

    Log.print("h:");
    Log.println(dht.readHumidity());
  
  }

}
