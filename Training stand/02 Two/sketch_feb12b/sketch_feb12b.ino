//
// Учебный стенд для занятий по радиосвязи.
//
// Определение температуры и передача по радиоканалу
// Принимающее устройство
// 
// Результат выводится на экран компьютера
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

    if ( strncmp(s, "t:", 2) == 0 ) {

      // Если получены данные из датчика температуры, то отобразить на экран компьютера
      
      Serial.print("Температура: ");
      Serial.print(s+2);
      Serial.println(" °C");

      // Cигнальный светодиод
      
      digitalWrite(PIN_LED, HIGH);
      delay(300);
      digitalWrite(PIN_LED, LOW);
    
    }

  }

}
