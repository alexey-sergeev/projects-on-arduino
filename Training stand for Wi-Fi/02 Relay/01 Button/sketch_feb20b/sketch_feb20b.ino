// 
// Учебный стенд для занятий по Wi-Fi
//
// Управление реле через Интернет
// Базовое устройство (кнопка)
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Pin нагрузки

#define PIN_RELAY 0

// Pin кнопки

#define PIN_BUTTON 5



void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

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
  
}



void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Отслеживание нажатий кнопки

  static bool f = true;
  static uint32_t timer = 0;

  if ( !digitalRead(PIN_BUTTON) && millis() - timer > 300 ) {
    
    timer = millis();

    Serial.println("Кнопка нажата");
    digitalWrite(PIN_RELAY, ( f ) ? HIGH : LOW);
    f = !f;

  }
  
}

