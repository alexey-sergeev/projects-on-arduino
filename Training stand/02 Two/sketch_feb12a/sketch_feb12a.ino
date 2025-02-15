//
// Учебный стенд для занятий по радиосвязи.
//
// Определение температуры и передача по радиоканалу
// Передающее устройство (задание номер два и задание номер три имеют один и тот же код)
// 
// Февраль 2025 г.
// 1.0 v
//


#define PIN_SENSOR 8
#define PIN_LED 12
#define PIN_RADIO 4

// Библиотека датчика температуры 
#include "DHT.h"
DHT dht(PIN_SENSOR, DHT11);

// Библиотека радиомодуля - TX 
#include <Gyver433.h>
Gyver433_TX<PIN_RADIO> tx;



void setup() {

  Serial.begin(9600);
  Serial.println("Hello");

  pinMode(PIN_LED, OUTPUT);

  dht.begin();

}


void loop() {

  // Вывести температуру

  float t = dht.readTemperature();

  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" °C");

  // Включить сигнальный светодиод

  digitalWrite(PIN_LED, HIGH);

  // Обработка данных перед передачей

  char value[] = "-00.00";
  dtostrf(t, 4, 2, value);
  char data[] = "t:-00.00";
  strcpy(data + 2, value); 
  
  // Передача данных по радиоканалу

  tx.sendData(data);

  // Выключить сигнальный светодиод
  
  delay(300);
  digitalWrite(PIN_LED, LOW);
 
  delay(3000);
  
}
