#define DISPLAY_LEN 10
#define MENU_LEN 5

#define MENU_TIME_2 0
#define MENU_TIME_1 1
// #define MENU_CLOCK 2
#define MENU_TEMP_1 2
#define MENU_TEMP_2 3
#define MENU_WIFI 4

#define FILE_TIMER_1 "/db/timer1.bin"
#define FILE_TIMER_2 "/db/timer2.bin"



#include <Ticker.h>
Ticker flipper;

uint8_t nMenu = 0;
uint32_t timer1;
uint32_t timer2;

bool flagMessage0 = false;
bool flagMessage1 = false;


class Relay {

public:

  void init() {

    pinMode(PIN_RELAY, OUTPUT);
    // digitalWrite(PIN_RELAY, HIGH);
    pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
    pinMode(PIN_BUTTON_RELAY, INPUT_PULLUP);
    pinMode(PIN_BUTTON_FLOAT, INPUT_PULLUP);

    // button.attachCallback(changeMenu);

    timer1 = _read(FILE_TIMER_1);
    timer2 = _read(FILE_TIMER_2);

    // pln(timer1);
    // pln(timer2);

    // setStampZone(3);

    attachInterrupt(PIN_BUTTON_RELAY, _doOnOff, CHANGE);
    attachInterrupt(PIN_BUTTON_FLOAT, _doOnOff, CHANGE);
    // attachInterrupt(PIN_BUTTON_MENU, _changeMenu, CHANGE);

    flipper.attach(60, _changeTimer);
       
  }




  void run() {

    _doOnOff();
    _display();
    _changeMenu();
    _sentMessage();

    sTemperatureS_buf = weather.temp;
    sRelay_buf = flagStatus;
    sBarrel_buf = flagStatusBarrel;
    sTimer1_buf = timer1 / 60;
    sTimer2_buf = timer2;
    // timer1_buf = timer1;
    // timer2_buf = timer2;

    if ( flagResetCounter ) {

      _removeTimer2();
      flagResetCounter = false;

    }


  }




private:



  static void _changeMenu() {

    static uint32_t timerButton;
    static bool f = false;
   
    if ( millis() - timerButton > 200 && ! f && ! digitalRead(PIN_BUTTON_MENU) ) {
  
      timerButton = millis();
      f = true;

    }

    if ( millis() - timerButton > 2000 && f || digitalRead(PIN_BUTTON_MENU) && f ) {
   
      if ( millis() - timerButton > 2000 ) _removeTimer2();
      else if ( ++nMenu >= MENU_LEN ) nMenu = 0;
      
    }

    if ( digitalRead(PIN_BUTTON_MENU) && f ) f = false;


   // WiFi

    static bool ff = false;

    if ( WiFi.status() != WL_CONNECTED && ! ff ) {  

      ff = true;
      nMenu = MENU_WIFI; 

    }
   
    if ( WiFi.status() == WL_CONNECTED && ff ) ff = false;


  }




  void _display() {

    #ifdef SET_DISPLAY

      static String s;
      s.reserve(DISPLAY_LEN);
      char tc[] = "000.0";
      // char pr[] = " ";

      // time_t tNow = time(NULL);
      // uint32_t t = (uint32_t) tNow + GMT;
      uint32_t t = (uint32_t)time(NULL) + GMT;
      uint32_t tt;

      // static uint8_t nMenuOld;
      // if ( nMenuOld != nMenu ) disp.power(true);
      // nMenuOld = nMenu;

      switch (nMenu) {

        case MENU_TIME_2:

          s = "";

          tt = trunc( timer2 / 60 );
          if ( tt <= 9 ) s += "0";
          s += (String) tt;
          
          tt = timer2 % 60;
          if ( tt <= 9 ) s += "0";
          s += (String) tt;

          display.print(s, true);

          break;

        case MENU_TIME_1:

          tt = trunc( timer1 / 60 );

          if ( tt <= 9999 ) s = "";
          if ( tt <= 999 ) s = "0";
          if ( tt <= 99 ) s = "00";
          if ( tt <= 9 ) s = "000";

          s += (String) tt;

          display.print(s);
          // pln(timer1);

          break;

        // case MENU_CLOCK:

        //   tt = 0;
        //   s = "";

        //   tt = (t % 86400) / 3600;
        //   if (tt < 10) s += "0";
        //   s += (String) tt;

        //   tt = (t % 3600) / 60;
        //   if (tt < 10) s += "0";
        //   s += (String) tt;

        //   // s += (String) now.hour();
        //   // s += (String) now.minute();
        //   display.print(s);



        //   break;

        case MENU_TEMP_1:

          dtostrf(sTemperatureB.get(), 1, 1, tc);
          s = (String) tc + "*";
          if ( s.length() < 5 ) s = " " + s;
          display.print(s, true);

          break;

        case MENU_TEMP_2:

          // s = forecast.showForecast();
          // s += "*";

          dtostrf(weather.temp, 1, 1, tc);
          s = (String) tc + "*";
          if ( s.length() < 5 ) s = " " + s;
          display.print(s, true);

          break;


        case MENU_WIFI:

          s = ( WiFi.status() != WL_CONNECTED ) ? "----" : WiFi.SSID();
          display.print(s);

          break;

          // default:
          // break;
      }


    #endif
  }




  IRAM_ATTR static void _doOnOff() {

    static uint32_t timerButton;
    static uint32_t timer;

    static bool f;
    static bool ff = true;
    bool flag;

    if ( millis() - timer > 3000 ) timer = millis();
    else {

      f = telegramBot.isOnOff();

      if ( f ) {

        if ( ff ) {

          flagMessage0 = true;
          // message0 = "Включаю насос по расписанию";
          // telegramBot.print("Включаю насос по расписанию");

          ff = false;
        }

      } else {

        ff = true;
      }
    }

    if ( flagOnOff == ON_OFF_DEFAULT ) flag = f;
    if ( flagOnOff == ON_OFF_ON ) flag = true;
    if ( flagOnOff == ON_OFF_OFF ) flag = false;

    if ( flagOnOff == ON_OFF_ON && f ) flagOnOff = ON_OFF_DEFAULT;
    if ( flagOnOff == ON_OFF_OFF && !f ) flagOnOff = ON_OFF_DEFAULT;


    if ( flagOnOff == ON_OFF_DELAYED )
      if ( millis() > timerDelayed ) flagOnOff = ON_OFF_OFF;
      else flag = true;

    // Кнопка
    if ( ! digitalRead(PIN_BUTTON_RELAY) && millis() - timerButton > 500 ) {

      timerButton = millis();

      if ( flag ) flagOnOff = ON_OFF_OFF;
      else flagOnOff = ON_OFF_ON;

      flag = !flag;

    }

    // Поплавок
    if ( ! digitalRead(PIN_BUTTON_FLOAT) ) {

      if ( flag ) flagMessage1 = true;

      // if ( flag ) telegramBot.print("Емкость заполнена. Выключаю насос");
      // if ( flag ) message1 = "Емкость заполнена. Выключаю насос";

      flag = false;
      flagOnOff = ON_OFF_OFF;
    }


    digitalWrite(PIN_RELAY, ( flag ) ? HIGH : LOW);
    // digitalWrite(PIN_RELAY, ( !flag ) ? HIGH : LOW);

    telegramBot.setStatus(flag);
    telegramBot.setStatusBarrel(digitalRead(PIN_BUTTON_FLOAT));

  }



  void _sentMessage() {

    if ( flagMessage0 ) telegramBot.print("Включаю насос по расписанию");
    if ( flagMessage1  ) telegramBot.print("Емкость заполнена. Выключаю насос");

    flagMessage0 = false;
    flagMessage1 = false;

    // message0 = "";
    // message1 = "";

  }





  static void _removeTimer2() {

    // pln("@@@");
    timer2 = 0;
    _save(FILE_TIMER_2, timer2);
     
  }


  
  
  
  static void _changeTimer() {

    if ( flagStatus ) {

      ++timer1;
      ++timer2;

      _save(FILE_TIMER_1, timer1);
      _save(FILE_TIMER_2, timer2);

    }

  }




  static void _save(String path, uint32_t t) {

    File file = storage.open(path, "w");

    p("RELAY: Insert timer ");
    p(t);
    p(" into ");
    p(path);
    pln();

    if ( !file ) {

      pln("RELAY: Failed to open file for appending");
      return;

    }

    if ( !file.print((String) t) ) pln("RELAY: Append failed");

    file.close();
  }





  uint32_t _read(String path) {

    File file = LittleFS.open(path, "r");

    if (!file) {
      pln("RELAY: Failed for reading");
      return 0;
    }

    char c[10];
    uint8_t n = 0;

    while (file.available()) { c[n++] = (char)file.read(); }
    c[n] = 0;

    p("RELAY: Read timer ");
    p(c);
    p(" from ");
    p(path);
    pln();

    file.close();

    return atol(c);
  }







};

Relay relay;
