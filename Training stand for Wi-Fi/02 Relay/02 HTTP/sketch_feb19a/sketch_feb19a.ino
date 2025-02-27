// 
// Учебный стенд для занятий по Wi-Fi
//
// Управление реле через Интернет
// Веб версия (HTTP)
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
  
  // Инициализация веб-сервера

  HTTP.on("/", [](){HTTP.send(200, "text/html", home());});
  HTTP.on("/on", [](){HTTP.send(200, "text/html", on());});
  HTTP.on("/off", [](){HTTP.send(200, "text/html", off());});
  HTTP.onNotFound([](){HTTP.send(404, "text/plain", "Not Found");});
  HTTP.begin();

}



void loop() {

  // Сигнальный светодиод
  
  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Помощник веб-сервера
  
  handle();

}


// Помощники

void handle() {
  HTTP.handleClient(); 
}



String home() {
  return html(0);
}



String on() {
  digitalWrite(PIN_RELAY, HIGH);
  // digitalWrite(PIN_RELAY, LOW); // только модуля расширения для ESP-01/ESP-01s
  return html(1);
}



String off() {
  digitalWrite(PIN_RELAY, LOW);
  // digitalWrite(PIN_RELAY, HIGH); // только модуля расширения для ESP-01/ESP-01s
  return html(2);
}



// 
// Создание страницы HTML
// 

// String html() {
String html(uint8_t n) {

  String s = "";

  s += "<!doctype html>\n";
  s += "<html><head>\n";
  s += "<meta charset=\"utf-8\">\n";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  s += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
  s += "<title>Реле</title></head><body>\n";
  
  s += "<div class=\"container\"><div class=\"row p-2\">\n";
  
  s += "<div class=\"d-grid gap-2 col-sm-6 col-lg-4 pt-3 mx-auto\">\n";


  s += "<a class=\"btn mx-2 ";
  s += ( n == 1 ) ? "btn-primary" : "btn-secondary";
  s += "\" href=\"/on\" role=\"button\">On</a>\n";

  s += "<a class=\"btn mx-2 ";
  s += ( n == 2 ) ? "btn-primary" : "btn-secondary";
  s += "\" href=\"/off\" role=\"button\">Off</a>\n";

  s += "</div>\n";
  
  s += "</div></div>\n";
  s += "</body></html>\n";

  return s;

}