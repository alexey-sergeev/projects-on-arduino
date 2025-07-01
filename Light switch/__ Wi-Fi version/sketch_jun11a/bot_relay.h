#include "bot_schedule.h"

// #define RELAY_PIN 4
// #define BUTTON_PIN 0
// #define FLOAT_PIN 3


#define COMMAND_ON 200
#define COMMAND_OFF 201
#define COMMAND_OFF_DELAYED 202
#define COMMAND_STATUS 203
#define COMMAND_STATUS_BARREL 204
#define COMMAND_DISPLAY 205
#define COMMAND_DISPLAY_RAW 206
#define COMMAND_WEB_SITE 207
#define COMMAND_RESET_COUNTER 208

#define ON_OFF_DEFAULT 0
#define ON_OFF_ON 1
#define ON_OFF_OFF 2
#define ON_OFF_DELAYED 3

uint8_t flagOnOff = ON_OFF_DEFAULT;
uint32_t timerDelayed = 0;
bool flagStatus = false;
bool flagStatusBarrel = false;
bool flagResetCounter = false;
// uint32_t timer1_buf;
// uint32_t timer2_buf;

class bot_relay : public bot_schedule {

  public:

    void init() {

      bot.attach(newMsg);
      _initEEPROM();
      _initSchEEPROM();

      // Получить точное время из Интернета
      configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    }



    void run() {

      tick();

      #ifdef SET_UPTIME 

        static bool f = true;

        if ( f ) {
        
          bot.sendMessage("Старт", _adminToString());
          f = false;

        }

      #endif


    }



    // Обработчик сообщений бота

    static void newMsg(FB_msg& msg) {

      p(msg.userID);
      p(", ");
      p(msg.username);
      p(", ");
      pln(msg.text); 

      if ( ! sa.admin.equals("") ) {

        if ( msg.text == "/say_hello" ) _commandSayHello(msg);
        if ( msg.text == "/menu" ) _commandMenu(msg);

        if ( _isUser(msg.userID) ) {

          if ( msg.text == "/list" ) _commandList(msg);
          if ( msg.text == "/i" ) _commandI(msg);
          if ( msg.text == "/on" ) _commandOn(msg);
          if ( msg.text == "/off" ) _commandOff(msg);
          if ( msg.text == "/off_delayed" || command == COMMAND_OFF_DELAYED ) _commandOffDelayed(msg);
          if ( msg.text == "/status" ) _commandStatus(msg);
          if ( msg.text == "/status_barrel" ) _commandStatusBarrel(msg);
          if ( msg.text == "/find_schedule" ) _commandFind(msg);
          if ( msg.text == "/display" ) _commandDisplay(msg);
          if ( msg.text == "/display_raw" ) _commandDisplayRaw(msg);
          if ( msg.text == "/website" ) _commandWebSite(msg);
          if ( msg.text == "/reset_counter" ) _commandResetCounter(msg);

          if ( _isAdmin(msg.userID) ) {

            if ( msg.text == "/add_schedule" || command == COMMAND_ADD ) _commandAdd(msg);
            if ( msg.text == "/edit_schedule" || command == COMMAND_EDIT ) _commandEdit(msg); 

            if ( msg.text == "/delete_user" || command == COMMAND_DELETE ) _commandDelete(msg);
            if ( msg.text == "/invitation" ) _commandInvitation(msg);
            if ( msg.text == "/q" || command == COMMAND_Q ) _commandQ(msg);

          }
        } else _setUser(msg);
      } else _setAdmin(msg);

    }



    static void print(String s) {
      
      bot.sendMessage(s, _userToString());
      
    }



    static void setStatus(bool f) {

      flagStatus = f;
      
    }


    static void setStatusBarrel(bool f) {

      flagStatusBarrel = f;
      
    }



  protected:


  


    static void _commandDisplay(FB_msg& msg) {

      command = COMMAND_DISPLAY;

      String s = "";
      s += "<b>Показания датчиков</b>\n\n";

      String data = sensors.creatingDataRow(N_NOW);
      data += " ";

      char c[data.length() + 1];

      for (int i = 0; i < 10; i++) c[i] = data.charAt(i);
      c[10] = 0;

      s += _getTime(c);
      s += "\n";
      s += "\n";

      uint16_t j = 0;

      for (int i = 10; i < data.length(); i++) {
        
        if ( data.charAt(i) == ' ' ) {

          c[j] = 0;
          j = 0;

          if ( c[0] == 'c' && c[1] == 'a' ) s += "\n<b>Счетчики:</b>\n\n";

          s += _getIndicator(c);
            
        } else c[j++] = data.charAt(i);

      }

      // s += "\n\n<b>Счетчики:</b>\n\n";
      
      // s += "Суточный: <b>";
      // s += timer2_buf;
      // s += " мин.</b>\n";
      
      // s += "Все время работы: <b>";
      // s += String ( timer1_buf / 60 );
      // s += " час.</b>\n";

      // #ifdef SET_FORECAST

      //   s +=_getForecast();

      // #endif

      #ifdef SET_UPTIME 

        s += "Рабочее время: <b>";
        s += (String) ( millis() / 3600000 );
        s += " час.</b>";

      #endif

      bot.setTextMode(FB_HTML);
      bot.sendMessage(s, msg.chatID);

    }


 
    static String _getForecast() {

      String s = "";

      #ifdef SET_FORECAST

        String s2 = forecast.showForecast('w');

        if ( s2 != "" ) {

          s += "\n<b>На улице:</b>\n\n";
          s += "Температура: <b>";
          s += s2;
          s += " °</b>\n";

        }

      #endif

      return s;

    }




    static String _getIndicator(char* c) {

      char s[100] = "";

      if ( strchr(c, ':') != 0 ) {

        char * key = strtok(c, ":");
        char * value = strtok(NULL, ":");

        String k = key;
        char k0 = k.charAt(0);
        char k1 = k.charAt(1);
        

        if ( k0 == 't' ) {

          strcat(s, "Температура: <b>");
          strcat(s, value);
          strcat(s, " °</b>");

        }

        else if ( k0 == 's' && k1 == 't' ) {

          strcat(s, "Температура на улице: <b>");
          strcat(s, value);
          strcat(s, " °</b>");
          
        }

        else if ( k0 == 'r' && k1 == 'r' ) {

          strcat(s, "Статус насоса: <b>");
          // strcat(s, value);
          strcat(s, ( value[0] == '1') ? "включен" : "выключен" );
          strcat(s, "</b>");
          
        }

        else if ( k0 == 'b' && k1 == 'b' ) {

          strcat(s, "Статус ёмкости: <b>");
          // strcat(s, value);
          // strcat(s, ( value[0] == '0') ? "off" : "on" );
          strcat(s, ( value[0] == '1') ? "заполнена" : "не заполнена" );
          strcat(s, "</b>");
          
        }

        else if ( k0 == 'c' && k1 == 'b' ) {

          strcat(s, "Суточный: <b>");
          // value[strchr(s, '.')] = 0;
          char * v = strtok(value, ".");
          strcat(s, v);
          strcat(s, " мин.</b>");
          
        }

        else if ( k0 == 'c' && k1 == 'a' ) {

          strcat(s, "Все время работы: <b>");
          // value[strchr(s, '.')] = 0;
          char * v = strtok(value, ".");
          strcat(s, v);
          // strcat(s, value);
          strcat(s, " час.</b>");
          
        }

        else {

          strcat(s, "value: <b>");
          strcat(s, value);
          strcat(s, "</b>");

        }

        strcat(s, "\n");

      }

      return (String) s;

    }





    static String _getTime(char* c) {

      String s = "Время: <b>";
      
      uint32_t t = atol(c) + GMT;

      uint8_t tt = 0; 
      
      tt = ( t % 86400 ) / 3600;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      s += ":";

      tt = ( t % 3600 ) / 60;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      s += ":";

      tt = t % 60;
      if ( tt < 10 ) s += "0";
      s += (String) tt;

      s += "</b>";

      return s;
      
    }




 
    static void _commandDisplayRaw(FB_msg& msg) {

      command = COMMAND_DISPLAY_RAW;

      String s = sensors.creatingDataRow();

      bot.setTextMode(FB_HTML);
      bot.sendMessage(s, msg.chatID);

      p("BOT (bot): "); 
      pln(s); 


    }



    static void _commandWebSite(FB_msg& msg) {

      command = COMMAND_WEB_SITE;

      #ifdef SET_HTTP

        String s;

        s += "Убедитесь, что вы подключены к той же сети, что и метеостанция: <b>";
        s += WiFi.SSID();
        s += "</b>\n\n";
        
        #ifdef SET_ID_DEVICE
         
          s += "ID устройства: <b>";
          s += String (idDevice(), HEX);
          s += "</b>\n\n";

        #endif
      
        s += "http://";

        s += WiFi.localIP().toString();

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);
       
      #endif

    }


 




    static void _commandMenu(FB_msg& msg) {

      command = COMMAND_MENU;

      if ( _isUser(msg.userID) ) {
        
        bool f = false;
        if ( _isAdmin(msg.userID) ) f = true;

        String s = "";

        s += "<b>Меню</b>\n\n";

        if (f) s += "<b>Реле:</b>\n\n";

        s += "/on — Включить\n";
        s += "/off — Выключить\n";
        s += "/off_delayed — Выключить с задержкой\n\n";

        s += "/status — Состояние насоса\n";
        s += "/status_barrel — Состояние ёмкости\n\n";

        s += "/display — Вывести показания датчиков\n";
        s += "/website — Вывести адрес веб-сайта\n";
        s += "/reset_counter — Обнулить счётчик\n";

        if (f) s += "\n<b>Расписание:</b>\n\n";
        if (!f) s += "\n<b>Настройки:</b>\n\n";

        s += "/find_schedule — Узнать расписание\n";
        if (f) s += "/add_schedule — Добавить\n";
        if (f) s += "/edit_schedule — Редактировать\n";

        if (f) s += "\n<b>Пользователь:</b>\n\n";

        s += "/list — Список пользователей\n";
        if (f) s += "/invitation — Создать приглашение\n";
        if (f) s += "/delete_user — Удалить\n";

        s += "\n<b>Устройство:</b>\n\n";

        s += "/say_hello — Поздороваться\n";

        s += "/i — Имя Wi-Fi\n";
        if (f) s += "/q — Cброс настроек\n";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);

      } else bot.sendMessage("Какой пин-код?", msg.chatID);

    }



    static void _commandOn(FB_msg& msg) {

      command = COMMAND_ON;
      flagOnOff = ON_OFF_ON;

      bot.sendMessage("On", msg.chatID);

    }



    static void _commandOff(FB_msg& msg) {

      command = COMMAND_OFF;
      flagOnOff = ON_OFF_OFF;

      bot.sendMessage("Off", msg.chatID);

    }



    static void _commandOffDelayed(FB_msg& msg) {

      static uint8_t n;

      if ( command != COMMAND_OFF_DELAYED ) n = 0; 
      command = COMMAND_OFF_DELAYED;

      if ( n == 0 ) {
      
        String s = "";
        s += "Как долго будет длиться задержка? (мин)";

        bot.sendMessage(s, msg.chatID);

        n = 1;

      } else if ( n == 1 ) {

        String s = "";

        uint16_t t = msg.text.toInt();

        if ( t || msg.text == "0" ) {

          s += "Готово! Бот отключит удлинитель через ";
          s += (String) t;
          s += " минут.";
          
          bot.sendMessage(s, msg.chatID);
          
          command = 0;

          flagOnOff = ON_OFF_DELAYED; 
          timerDelayed = millis() + t * 60000;

        } else if ( ! msg.text.startsWith("/") ) bot.sendMessage("Формат неправильный. Попробуйте ещё раз.", msg.chatID);
        
      }


    }



    static void _commandStatus(FB_msg& msg) {

      command = COMMAND_STATUS;

      String s = "";
      bool f = flagStatus;

      s += ( f ) ? "Насос включен" : "Насос выключен";

      if ( flagOnOff == ON_OFF_DEFAULT && f ) s += " (по расписанию)"; 
      if ( flagOnOff == ON_OFF_DELAYED && f ) {

        uint32_t t = trunc(( timerDelayed - millis() ) / 60000) + 1;
        
        s += " (с задержкой, осталось ";
        s += (String) t;
        s += " мин.)";

      }

      bot.sendMessage(s, msg.chatID);
   
    }



    static void _commandStatusBarrel(FB_msg& msg) {

      command = COMMAND_STATUS_BARREL;

      String s = "";

      s += ( ! flagStatusBarrel ) ? "Емкость заполнена" : "Емкость не заполнена";

      bot.sendMessage(s, msg.chatID);
   
    }


    static void _commandResetCounter(FB_msg& msg) {

      command = COMMAND_RESET_COUNTER;

      String s = "Готово";
      flagResetCounter = true;

      bot.sendMessage(s, msg.chatID);
   
    }


  private:


};

bot_relay telegramBot; 
