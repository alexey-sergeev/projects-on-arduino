// 
// Конфигурация 
// 


#define HELLO "conf"
// #define CONF


// Wi-Fi

#define SET_WIFI
#define SET_LED_REVERSE_ORDER
// #define SSID_0 ""
// #define PASS_0 ""

// #define SET_OTA
// #define PASSWORD_OTA "" 

// Сервисы

#define SET_BOT
#define SIZE_USER 25
#define KEY "sa:iYEp"
#define KEY_SCH "sa:LaJR"
// #define BOT_TOKEN "" // Смотри https://t.me/BotFather

#define SET_DB

#define SET_HTTP
// #define LOGIN_FTP ""
// #define PASSWORD_FTP "" 

#define SET_MQTT
// #define MQTT_TOPIC "ESP/Data"
// #define MQTT_SERVER "" // Смотри https://clusterfly.ru/ или https://www.wqtt.ru/
// #define MQTT_PORT 
// #define MQTT_USER ""
// #define MQTT_PASSWORD "" 

#define SET_TELNET 
// #define SET_UPTIME 
// #define SET_UPDATE


// Реле

#define PIN_RELAY 0
#define PIN_BUTTON_MENU 13 // Меню
#define PIN_BUTTON_RELAY 1
#define PIN_BUTTON_FLOAT 14



// Дисплей

// #define SET_DISPLAY
// #define PIN_CLK 16
// #define PIN_DIO 14
// #define CLK_PIN 16
// #define DIO_PIN 12
#define CLK_PIN 14
#define DIO_PIN 16
// #define LAT_PIN 14


// Прогноз погоды

#define SET_FORECAST
// #define FORECAST_APPID "" // Смотри https://openweathermap.org/


// Датчик температуры

// #define SET_TEMPERATURE_B
// #define PIN_TEMPERATURE_B 5
// // #define SET_BMP 
// #define PREFIX_TEMPERATURE_B "t10"


#define SET_TEMPERATURE_S
#define PREFIX_TEMPERATURE_S "st10"

#define SET_RELAY
#define PREFIX_RELAY "rr10"

#define SET_BARREL
#define PREFIX_BARREL "bb10"

#define SET_TIMER_1
#define PREFIX_TIMER_1 "ca10"

#define SET_TIMER_2
#define PREFIX_TIMER_2 "cb10"


// Параметры датчиков

#define DATA_BUF_COUNT 64

#define SAMPLE_SIZE 8 // День
#define SAMPLE_SIZE_2 32 // Месяц
#define SAMPLE_SIZE_3 4 // Год


// #define MY_SDA 14
// #define MY_SLC 16

