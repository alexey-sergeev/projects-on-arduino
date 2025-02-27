// 
// Учебный стенд для занятий по Wi-Fi
//
// Определение температуры и влажности
// Результат выводится через MQTT в приложение для телефона или специальный веб-сайт.
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека MQTT-брокера

#include <PubSubClient.h>
WiFiClient net;
PubSubClient client(net);

// Получить настройки можно в https://clusterfly.ru/

#define MQTT_SERVER "" 
#define MQTT_PORT 9991
#define MQTT_USER ""
#define MQTT_PASSWORD "" 

#define MQTT_CIENT_ID "user_xxxxxxxx_dht11" // MQTT_USER + какая строка
#define MQTT_PUBLISH_T "user_xxxxxxxx/dht/t" // MQTT_USER + /dht/t
#define MQTT_PUBLISH_H "user_xxxxxxxx/dht/h" // MQTT_USER + /dht/h

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
  
  // Инициализация MQTT брокера
      
  client.setServer(MQTT_SERVER, MQTT_PORT);

  // Инициализация датчика

  dht.begin();

}




void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Подключение к MQTT-брокеру

  if ( !client.connected() ) {

    if ( client.connect(MQTT_CIENT_ID, MQTT_USER, MQTT_PASSWORD) ) {
      
      Serial.println("Успешное подключение к брокеру MQTT");

    } else {
      
      Serial.println("Сбой соединения с брокером MQTT");
      delay(5000);

    }
  
  }
  
  client.loop();

  // Отправка MQTT-брокеру  

  static uint32_t timer = 0;

  if ( millis() - timer > 10000 ) {
  
    timer = millis();
  
    Serial.print("Tемпература: ");
    Serial.println(dht.readTemperature());

    if ( !client.publish(MQTT_PUBLISH_T, ((String) dht.readTemperature()).c_str()) ) Serial.println("Publish failed (t)");

    Serial.print("Влажность: ");
    Serial.println(dht.readHumidity());

    if ( !client.publish(MQTT_PUBLISH_H, ((String) dht.readHumidity()).c_str()) ) Serial.println("Publish failed (h)");
  
  }
  
}

