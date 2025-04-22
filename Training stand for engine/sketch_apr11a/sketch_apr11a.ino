// 
// Учебный стенд для занятий управление моторами
//
// Базовая версия
// 
// Апрель 2025 г.
// 1.0 v
//

int pinA = 7;
int pinB = 6;

int pinC = 4;
int pinD = 5;

bool f = true;


void setup() {

  Serial.begin(115200);
  Serial.println("Hello");

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);

  pinMode(2, INPUT);

  // Передвинуть рычаг на место
  
  digitalWrite(pinA, 1);
  analogWrite(pinB, 180);   
  while ( digitalRead(2) );
  analogWrite(pinB, 255); 
  
  // Следить за датчиком
  
  attachInterrupt(0, isr, FALLING);

  // Включить вентилятор

  digitalWrite(pinC, 0);
  analogWrite(pinD, 255);  

  delay(3000);

}



void isr() {

  if ( f ) {

    Serial.println("1 @ рычаг на месте");

    f = false;

    // Остановить рычаг

    digitalWrite(pinA, 0);
    analogWrite(pinB, 0);   

  } else {

    Serial.println("2 @ ложное срабатывание датчика");

  }
  
}



void loop() {

  Serial.println("3 @ рычаг повернуть вниз");

  digitalWrite(pinA, 0);
  analogWrite(pinB, 150);   
  delay(50);
  analogWrite(pinB, 70);

  delay(350);
  f = true;


  Serial.println("4 @ опускание воду");

  digitalWrite(pinA, 0);
  analogWrite(pinB, 0);  

  delay(1000);


  Serial.println("5 @ рычаг повернуть вверх");

  digitalWrite(pinA, 1);
  analogWrite(pinB, 50);   
  delay(50);

  analogWrite(pinB, 200);   
  delay(1000);


  Serial.println("6 @ выдувание пузыря");

  delay(1000);

}
