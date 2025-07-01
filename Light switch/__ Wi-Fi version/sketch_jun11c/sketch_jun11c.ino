// Проверка фоторезистора

#define PIN 3

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN, INPUT);

}


void loop() {
  
  if ( digitalRead(PIN) ) digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);

}
