//
// Учебный стенд для занятий по радиосвязи
// 
// При нажатии на кнопку на другом устройстве загорается светодиод.
// Принимающее устройство
// 
// Февраль 2025 г.
// 1.0 v
//


#define PIN_LED 12
#define PIN_RADIO 2
#define BUFFER_LEN 16

// Библиотека радиомодуля - RX 
#include <Gyver433.h>
Gyver433_RX<PIN_RADIO, BUFFER_LEN> rx;



void setup() {

  Serial.begin(9600);
  Serial.println("Hello");

  pinMode(PIN_LED, OUTPUT);
  attachInterrupt(0, isr, CHANGE);

}



void isr() {

  // Получение данных (вызывается в прерывании)
  rx.tickISR();

}



void loop() {

  if ( rx.gotData() ) {

    // Приняты данные по радиосвязи
    
    // Обработка
    char s[rx.size + 1];
    strncpy(s, (char*) rx.buffer, rx.size);
    s[rx.size] = 0;

    Serial.print("Данные: ");
    Serial.println(s);

    if ( strcmp(s, "b:1") == 0 ) {

      // Если пришли данные из кнопки, то включить светодиод

      Serial.println("Включаю светодиод");

      digitalWrite(PIN_LED, HIGH);
      delay(300);
      digitalWrite(PIN_LED, LOW);
    
    }

  }

}
