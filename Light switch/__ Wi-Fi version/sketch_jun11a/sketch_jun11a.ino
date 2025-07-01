// Проект "Реле"
// (Выключатель света с таймером и датчиком освещенности. Версия с Wi-Fi)
// Версия 
// 

#define VERSION "2.0"

#include "conf.h" 


#include "passwd.h"
#include "func.h"
#include "mString.h"

#include "sensor.h"

#define INTERVAL_SENSOR_READIGS 1000 // 1 * 1000
#define INTERVAL_SAVE_TO_DB_DAY 120000 // 2 * 60 * 1000
#define INTERVAL_PUBLISH_NOW 20000 // 10 (должно больше, чем INTERVAL_PRINT_TX_NOW) * 1000
#define INTERVAL_PRINT_TX_NOW 10000 // 5 (должно меньше, чем INTERVAL_PUBLISH_NOW) * 1000

#define PREFIX_NOW "$Now:"
#define PREFIX_DAY "$Day:"
#define PREFIX_MONTH "$Mon:"
#define PREFIX_YEAR "$Yea:"

#define GMT 10800 // 60 * 60 * 3

#include "all_sensors.h"

#ifdef SET_BUTTON 
  #include "button.h"
#endif

#ifdef SET_DISPLAY 
  #include "display.h"
#endif

#ifdef SET_WIFI
  #include "wifi.h"
#endif

#ifdef SET_FORECAST 
  #include "forecast.h"
#endif

#ifdef SET_DB 
  #include "fs.h"
  #include "ftp.h"
  #include "db.h"
#endif

#ifdef SET_HTTP 
  #include "http.h"
#endif

#ifdef SET_BOT 
  #include "bot_relay.h"
#endif

#ifdef SET_MQTT 
  #include "mqtt.h"
#endif

#include "relay.h"



void setup() {


  Serial.begin(115200);

  Serial.println();
  Serial.print("Hello (");
  Serial.print(HELLO);
  Serial.println(")");

  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  Serial.print("Version: ");
  Serial.println(VERSION);

  #ifdef MY_SDA
    Wire.begin(MY_SDA, MY_SLC);
  #endif

  #ifdef SET_WIFI
    // wifi.initWiFi();
    wifi.initMulti();
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  #endif

  #ifdef SET_TELNET 
    Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
    Log.begin();
  #endif

  #ifdef SET_BOT 
    telegramBot.init();
  #endif

  #ifdef SET_DB
    storage.init();
    ftp.init();
    db.init();
  #endif

  #ifdef SET_HTTP
    http.init();
  #endif

  #ifdef SET_MQTT
    mqtt.init();
  #endif

  #ifdef SET_DISPLAY 
    display.init();
  #endif

  #ifdef SET_BUTTON 
    button.init();
  #endif

  #ifdef SET_FORECAST 
    forecast.init();
  #endif

  sensors.init();
  relay.init();

}



void loop() {


  // Проверка wi-fi
  #ifdef SET_WIFI

    wifi.runMulti();
  
  #endif

  #ifdef SET_TELNET 

    Log.loop();

  #endif

  // Проверка MQTT
  #ifdef SET_MQTT
  
    mqtt.run();
  
  #endif

  // Ввод показаний датчиков
  sensors.run();
  relay.run();

  #ifdef SET_BOT 

    // Писать и читать сообщения бота 
    // telegramBot.tick();
    telegramBot.run();
  
  #endif

  #ifdef SET_DISPLAY 

    display.run();

  #endif

  #ifdef SET_FORECAST 

    forecast.run();

  #endif

  #ifdef SET_HTTP

    // Помощник серверов
    http.handle();

  #endif

  #ifdef SET_DB

    ftp.handle();
    
    // Вставка данных
    // Интервал обновления:
    //  - 2 минуты (дневная таблица)
    //  - 1 час (месячная таблица)
    //  - 3 часа (годовая таблица)
    db.insert();

    // Очистка базы данных
    // Один раз в день с 3 до 4 часов, но один раз
    db.purge();

  #endif

}


