#include "config.h"

#if defined(ESP32) || defined(ESP8266)
    AsyncWebServer server(80);
#endif

/** Объявление классов */
MyTimer timer;
MyUtils utils;
Window window(MOTOR_IN_1_PIN, MOTOR_IN_2_PIN);

/** Pins */

/** Настройка пинов для ESP32_C3 */
#if defined(ESP32)
    int8_t MOTOR_IN_1_PIN = 0;
    int8_t MOTOR_IN_2_PIN = 1;
    // uint8_t WATER_IN_3_PIN = 20;
    // uint8_t WATER_IN_4_PIN = 10;
    uint8_t LED_STRIP_ONE_PIN = 5;
    uint8_t LED_STRIP_TWO_PIN = 6;
    uint8_t LED_STRIP_THREE_PIN = 7;
    uint8_t SOIL_MOISTURE_PIN = 4;
    uint8_t THERMOMETER_PIN = 3;
    uint8_t LIGHT_SENSOR_PIN = 21;
#endif

// #if defined(ESP8266)
// здесь можно указать пины для ESP8266, если планируется поддержка этой платформы
// #endif

/** General settings */
uint32_t controlTime = 1000; // Интервал проверки показаний датчиков (в миллисекундах), при котором выполняется управление чем-либо (открытие окна, включение полива, включение освещения и т.д.)
float controlTemperature = 24.0;
uint16_t soilMoistureDryValue = 3750;
uint16_t soilMoistureWetValue = 3600;
bool isLedStripsOn = false;
bool isWaterOn = false;

uint32_t &controlTimeRef = controlTime;
float &controlTemperatureRef = controlTemperature;
uint16_t &soilMoistureDryValueRef = soilMoistureDryValue;
uint16_t &soilMoistureWetValueRef = soilMoistureWetValue;
bool &isLedStripsOnRef = isLedStripsOn;
bool &isWaterOnRef = isWaterOn;

/** WiFi config */
char SSID[21] = "greenhouse";
char PASSWORD[21] = "";

/** Sensors */
bool hasSoilMoistureSensor = false;
bool hasPhotoSensor = false;
bool hasTemperatureSensor = false;

bool &hasSoilMoistureSensorRef = hasSoilMoistureSensor;
bool &hasPhotoSensorRef = hasPhotoSensor;
bool &hasTemperatureSensorRef = hasTemperatureSensor;

/** Time config */
bool isWorkTimeEnabled = false;
WorkTime start = WorkTime(); // Время начала работы аппарата (09:00)
WorkTime end = WorkTime(); // Время окончания работы аппарата (20:00)

bool &isWorkTimeEnabledRef = isWorkTimeEnabled;
WorkTime &WTStartRef = start;
WorkTime &WTEndRef = end;
