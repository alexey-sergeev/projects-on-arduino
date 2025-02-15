//
// Учебный стенд для занятий по радиосвязи.
//
// При нажатии на кнопку на другом устройстве загорается светодиод.
// Передающее устройство
// 
// Февраль 2025 г.
// 1.0 v
//


#define PIN_BUTTON A0
#define PIN_LED 12
#define PIN_RADIO 4

// Библиотека радиомодуля - TX 
#include <Gyver433.h>
Gyver433_TX<PIN_RADIO> tx;



void setup() {

  Serial.begin(9600);
  Serial.println("Hello");

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

}



void loop() {

  if ( !digitalRead(PIN_BUTTON) ) { 

    // Кнопка была нажата
    
    Serial.println("Кнопка была нажата");
    digitalWrite(PIN_LED, HIGH);
  
    // Передача данных по радиоканалу
    tx.sendData("b:1");

    delay(300);
    digitalWrite(PIN_LED, LOW);

  } 

}
