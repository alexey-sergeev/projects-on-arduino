// 
// Обучающее устройство для изучения работы аналоговых датчиков на Arduino
// Улучшенная версия
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

  // Пишем на дисплее

  lcd.home();
  lcd.print("Hello");
  lcd.home();

  // Инициализация индикатор выполнения и смайлика

  progressBarInit();
  smileInit();

  // Очистить дисплей
  
  lcd.clear();

}



void loop() {

  // Считывание информации с датчика, пересчет в проценты

  int a = analogRead(A0);
  int p = map(a, 0, 1023, 0, 100);
  
  // Умная задержка
  
  static uint32_t timer;

  if ( millis() - timer > 500 ) {
  
    timer = millis();
    
    // Пишем в консоли

    Serial.print(p);
    Serial.print("%, Raw: ");
    Serial.println(a);

    // Пишем на дисплее

    lcd.setCursor(0, 0);  
    lcd.print(p);
    lcd.print("% ");

    lcd.setCursor(10, 0);  
    lcd.print("R:");
    lcd.print(a);
    lcd.print("    ");

    // Рисуем на индикаторе выполнения

    progressBarDraw(p);

  }

  // Обновить смайлик
  
  smileShow(p);

}




// Рисуем на индикаторе выполнения

void progressBarDraw(int p) {

  int edge = map(p, 0, 100, 0, 16);

  lcd.setCursor(0, 1); 

  for (int i = 0; i < edge; i++) lcd.write(0);
  for (int i = edge; i < 16; i++) lcd.write(1);
    
}



// Показываем смайлик

void smileShow(int p) {

  int n = map(p, 0, 100, 0, 7);

  lcd.setCursor(5, 0); 

  switch (n) {

    case 0 :
      // |(
      lcd.write( (bitRead(millis(), 12) == 0 && bitRead(millis() + 5000, 12) == 0 ) ? 58 : 5 );
      lcd.print("("); 
    break;
   
    case 1 : 
      // :(
      lcd.write( ( bitRead(millis(), 11) == 0 && bitRead(millis() + 5000, 11) == 0 ) ? 5 : 58 );
      lcd.print("("); 
    break;
   
    case 2 : 
      // :|
      lcd.write( ( bitRead(millis(), 11) == 0 && bitRead(millis() + 5000, 11) == 0 ) ? 5 : 58 );
      lcd.print("|");
    break;
   
    case 3 : 
    case 4 : 
    case 5 : 
      // :)
      lcd.write( ( bitRead(millis(), 11) == 0 && bitRead(millis() + 5000, 11) == 0 ) ? 5 : 58 );
      lcd.print(")"); 
    break;
   
    case 6 : 
    case 7 : 
      // %)
      lcd.write( ( bitRead( millis(), 9 ) == 0 ) ? 3 : 4 );
      lcd.print(")"); 
    break;

  }
    
}



// Инициализация индикатора выполнения

void progressBarInit() {

  // Новые символы 
  // https://maxpromer.github.io/LCD-Character-Creator/
  uint8_t rChar0[] = {0x1F, 0x1F, 0x1B, 0x1B, 0x1B, 0x1B, 0x1F, 0x1F};
  uint8_t rChar1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};

  lcd.createChar(0, rChar0);
  lcd.createChar(1, rChar1);

  lcd.setCursor(0, 1); 

  for (int i = 0; i < 16; i++) {
    lcd.write(255);
    delay(80);
  }

  lcd.setCursor(0, 1); 

  for (int i = 0; i < 16; i++) {
    lcd.write(32);
    delay(80);
  }

}



// Инициализация смайлика

void smileInit() {

  uint8_t rChar2[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};
  uint8_t rChar3[] = {0x00, 0x0C, 0x0C, 0x00, 0x06, 0x06, 0x00, 0x00};
  uint8_t rChar4[] = {0x00, 0x06, 0x06, 0x00, 0x0C, 0x0C, 0x00, 0x00};
  uint8_t rChar5[] = {0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00, 0x00};
  
  lcd.createChar(2, rChar2);
  lcd.createChar(3, rChar3);
  lcd.createChar(4, rChar4);
  lcd.createChar(5, rChar5);
  lcd.home();

}

