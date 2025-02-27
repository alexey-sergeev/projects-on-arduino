// 
// Учебный стенд для занятий по Wi-Fi
//
// Управление реле через Интернет
// Версия MQTT
// 
// Февраль 2025 г.
// 1.0 v
//


// Библиотека Wi-Fi

#include <ESP8266WiFi.h>

#define MY_SSID "fizmat"
#define MY_PASS ""

// Библиотека MQTT брокера

#include <PubSubClient.h>
WiFiClient net;
PubSubClient client(net);

// Получить настройки можно в https://clusterfly.ru/

#define MQTT_SERVER "" 
#define MQTT_PORT 9991
#define MQTT_USER ""
#define MQTT_PASSWORD "" 

#define MQTT_CIENT_ID "user_xxxxxxxx_relay" // MQTT_USER + какая строка
#define MQTT_SUBSCRIBE_RELAY "user_xxxxxxxx/relay/do" // MQTT_USER + /relay/do

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
  
  // Инициализация MQTT брокера
      
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(msgReceived);

}



void loop() {

  // Сигнальный светодиод

  digitalWrite(LED_BUILTIN, ( WiFi.status() == WL_CONNECTED && bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

  // Подключение к MQTT-брокеру

  if ( !client.connected() ) {

    if ( client.connect(MQTT_CIENT_ID, MQTT_USER, MQTT_PASSWORD) ) {
      
      Serial.println("Успешное подключение к брокеру MQTT");

      client.subscribe(MQTT_SUBSCRIBE_RELAY, 1);

    } else {
      
      Serial.println("Сбой соединения с брокером MQTT");
      delay(5000);

    }
  
  }
  
  client.loop();

}


// 
// Помощник MQTT
// 

void msgReceived(char* topic, byte* payload, unsigned int length) {

    String msg = "";
    for ( int i = 0; i < length; i++ ) msg += (char) payload[i];

    Serial.print("Получено сообщение [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(msg);

    if ( msg == "relay_on" ) on();
    if ( msg == "relay_off" ) off();

}



//
// Включить
//

void on() {

  Serial.println("On");
  digitalWrite(PIN_RELAY, HIGH);
// digitalWrite(PIN_RELAY, LOW); // только модуля расширения для ESP-01/ESP-01s
}



//
// Выключить
//

void off() {

  Serial.println("Off");
  digitalWrite(PIN_RELAY, LOW);
  // digitalWrite(PIN_RELAY, HIGH); // только модуля расширения для ESP-01/ESP-01s

}
