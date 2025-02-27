// 
// Учебный стенд для занятий по Wi-Fi
//
// Определение температуры и влажности
// Результат выводится на веб-сайте
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека веб-сервера

#include <ESP8266WebServer.h>
ESP8266WebServer HTTP(80);

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
  
  // Инициализация веб-сервера

  HTTP.on("/", [](){HTTP.send(200, "text/html", html());});
  HTTP.onNotFound([](){HTTP.send(404, "text/plain", "Not Found");});
  HTTP.begin();

  // Инициализация датчика

  dht.begin();

}



void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Помощник веб-сервера
  
  handle();

}




void handle() { HTTP.handleClient(); }




// 
// Создание страницы HTML
// 

String html() {

  String s = "";

  s += "<!doctype html>\n";
  s += "<html><head>\n";
  s += "<meta charset=\"utf-8\">\n";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  s += "<meta http-equiv=\"refresh\" content=\"5\">\n";
  s += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
  s += "<title>Показания</title></head><body>\n";
  
  s += "<div class=\"container\"><div class=\"row p-2\">\n";
  
  s += "<div class=\"col-sm-6 col-lg-4 pt-3\"><div class=\"card\">\n";
  s += "<div class=\"card-header\"><strong>Tемпература</strong></div>\n";
  s += "<div class=\"card-body\">\n";
  s += dht.readTemperature();
  s += " °C";
  s += "</div></div></div>\n";
  
  s += "<div class=\"col-sm-6 col-lg-4 pt-3\"><div class=\"card\">\n";
  s += "<div class=\"card-header\"><strong>Влажность</strong></div>\n";
  s += "<div class=\"card-body\">\n";
  s += dht.readHumidity();
  s += " %";
  s += "</div></div></div>\n";
  
  s += "</div></div>\n";
  s += "</body></html>\n";

  return s;

}
