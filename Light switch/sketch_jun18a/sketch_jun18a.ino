// Выключатель света с таймером и датчиком освещенности
// Версия 2.1 
// 


// 03

// #define PIN_RELAY 12 

// #define PIN_LED_R 9 
// #define PIN_LED_B 10
// #define PIN_LED_W 4

// #define PIN_SWITCH 3

// #define PIN_POTENTIOMETER_1 A3
// #define PIN_POTENTIOMETER_2 A1 
// #define PIN_PHOTORESISTOR A0



// 04

// #define PIN_RELAY 12

// #define PIN_LED_R 3 
// #define PIN_LED_B 11 
// #define PIN_LED_W 10  

// #define PIN_SWITCH 13

// #define PIN_POTENTIOMETER_1 A0
// #define PIN_POTENTIOMETER_2 A1 
// #define PIN_PHOTORESISTOR A2



// 05

#define PIN_RELAY A3

#define PIN_LED_R 3 
#define PIN_LED_B 13 
#define PIN_LED_W 10  

#define PIN_SWITCH 12

#define PIN_POTENTIOMETER_1 A0
#define PIN_POTENTIOMETER_2 A1 
#define PIN_PHOTORESISTOR A2





#define MIN 0 // 0
#define MAX 900000 // 15 * 60 * 1000
#define DELAY_SWITCH 60000 // 60 * 1000  
#define GAP 5 // 5% 



void setup() {

  Serial.begin(115200);
  Serial.println("Hello");

  pinMode(PIN_RELAY, OUTPUT);
  // digitalWrite(PIN_RELAY, HIGH); // Только для обратного реле

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_LED_W, OUTPUT);

  pinMode(PIN_SWITCH, INPUT_PULLUP);

  pinMode(PIN_POTENTIOMETER_1, INPUT);
  pinMode(PIN_POTENTIOMETER_2, INPUT);
  pinMode(PIN_PHOTORESISTOR, INPUT);

}



void loop() {

  Serial.println();

  static bool flag = false; // Relay 
 
  static bool flagLED = false; // LED B
  static bool flagLED2 = false; // LED B
  static uint32_t timer; // LED B

  static bool flagSwitch = false;



  // Основной цикл

  if ( ! digitalRead(PIN_SWITCH) ) {

    if ( flagSwitch ) flag = true;
    flagLED = false;
    timer = millis();

  } else {

    if ( flag ) flagLED = true;

  }

  digitalWrite(PIN_RELAY, ( flag ) ? HIGH : LOW ); 
  // digitalWrite(PIN_RELAY, ( ! flag ) ? HIGH : LOW );  // Только для обратного реле 
  
  Serial.print(flag);



  // Photoresistor

  int32_t p = map(analogRead(PIN_POTENTIOMETER_1), 0, 1023, 0, 100);
  // int32_t f = map(analogRead(PIN_PHOTORESISTOR), 0, 1023, 100, 0);
  int32_t f = map(analogRead(PIN_PHOTORESISTOR), 0, 1023, 0, 100);
  static int32_t timerSwitch = millis() - DELAY_SWITCH;

  if ( f > p + GAP && millis() - timerSwitch > DELAY_SWITCH  ) {

    timerSwitch = millis();
    flagSwitch = true;

  }

  if ( f <= p - GAP && millis() - timerSwitch > DELAY_SWITCH  ) {

    timerSwitch = millis();
    flagSwitch = false;
    flag = false;

  }

  Serial.print(" : ");
  Serial.print(millis() - timerSwitch);
  Serial.print(" : ");
  Serial.print(p);
  Serial.print(" : ");
  Serial.print(f);
  Serial.print(" : ");
  Serial.print(flagSwitch);



  // LED B

  uint32_t t = map(analogRead(PIN_POTENTIOMETER_2), 0, 1023, MIN, MAX);

  if ( millis() - timer > t ) {
    
    flag = false;
    flagLED = false;
    
  }

  if ( flagLED ) flagLED2 = ( ( millis() / (( map(millis() - timer, 0, t, 1, 100) < 75 ) ? 150 : 75) ) % 2 == 0 ) ? true : false;

  digitalWrite(PIN_LED_B, ( ( flagLED && flagLED2 ) || 
                            ( ! digitalRead(PIN_SWITCH) && !flag && f > p + GAP ) ||
                            ( ! digitalRead(PIN_SWITCH) && flag && f <= p - GAP && bitRead( millis(), 10 ) == 0 )
                            ) ? HIGH : LOW );  

  Serial.print(" : ");
  Serial.print(t);
  Serial.print(" : ");
  Serial.print(flagLED);



  // LED W 

  digitalWrite(PIN_LED_W, ( ! digitalRead(PIN_SWITCH) ) ? HIGH : LOW );  



  // LED R 

  digitalWrite(PIN_LED_R, ( digitalRead(PIN_SWITCH) && ! flagLED && bitRead( millis(), 10 ) == 0 ) ? HIGH : LOW );  
  // digitalWrite(PIN_LED_R, HIGH );  

}
