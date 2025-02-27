// 
// Учебный стенд для занятий по Wi-Fi
//
// Определение температуры и влажности
// Результат выводится на экран компьютера
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека датчика

#include "DHT.h"
DHT dht(0 /* PIN_SENSOR */, DHT11);




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
  
  // Инициализация датчика

  dht.begin();

}



void loop() {

  // Сигнальный светодиод
  
  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Выведение на экран

  static uint32_t timer = 0;

  if ( millis() - timer > 10000 ) {
  
    timer = millis();
  
    Serial.print("Tемпература: ");
    Serial.println(dht.readTemperature());

    Serial.print("Влажность: ");
    Serial.println(dht.readHumidity());
  
  }
  
}

