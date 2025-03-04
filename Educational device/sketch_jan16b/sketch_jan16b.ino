//
// Обучающее устройство для изучения работы аналоговых датчиков на Arduino
// Стандартная версия
//
// Январь 2025 г.
// 1.0 v
//


// Библиотека дисплея
// Адрес дисплея (0x3f, 0x27, 0х26, ...) и размер дисплея 16x2 (20x4, ...)
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup() {

  // Инициализация консоли

  Serial.begin(115200);
  Serial.println("Hello");

  // Инициализация дисплея

  lcd.init();
  lcd.backlight();

}



void loop() {

  // Считывание информации с датчика, пересчет в проценты

  int a = analogRead(A0);
  int p = map(a, 0, 1023, 0, 100);

  // Пишем в консоли

  Serial.print(p);
  Serial.print("%, Raw: ");
  Serial.println(a);

  // Пишем на дисплее

  lcd.setCursor(0, 0);
  lcd.print(p);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Raw: ");
  lcd.print(a);
  lcd.print("    ");

  // Задержка

  delay(500);
  
}
