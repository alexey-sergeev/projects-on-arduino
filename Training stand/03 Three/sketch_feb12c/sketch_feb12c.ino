//
// Учебный стенд для занятий по радиосвязи
//
// Определение температуры и передача по радиоканалу
// Принимающее устройство
// 
// Результат выводится на дисплее устройства
// 
// Февраль 2025 г.
// 1.0 v
//

#define PIN_LED 12
#define PIN_RADIO 2
#define BUFFER_LEN 16

// Библиотека радиомодуля 
#include <Gyver433.h>
Gyver433_RX<PIN_RADIO, BUFFER_LEN> rx;

// Библиотека дисплея
// Адрес дисплея (0x3f, 0x27, 0х26, ...) и размер дисплея 16x2 (20x4, ...)
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {

  Serial.begin(9600);
  Serial.println("Hello");

  pinMode(PIN_LED, OUTPUT);
  attachInterrupt(0, isr, CHANGE);

  // Инициализация дисплея

  lcd.init();
  lcd.backlight();

  lcd.print("Hello");

  delay(2000);
  
  lcd.clear();
  lcd.print("Temp: ");

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

      // Вывод на дисплее устройства

      lcd.setCursor(6, 0);
      lcd.print(s+2);
      lcd.print(" ");
      lcd.write(223);
      lcd.print("C   ");

      // Cигнальный светодиод

      digitalWrite(PIN_LED, HIGH);
      delay(300);
      digitalWrite(PIN_LED, LOW);
    
    }

  }

}
