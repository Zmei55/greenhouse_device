#include "config.h"

#if defined(ESP32) || defined(ESP8266)
    AsyncWebServer server(80);
#endif

GyverDS18Single term(THERMOMETER_PIN);

/** Pins */

/** Настройка пинов для ESP32_C3 */
#if defined(ESP32)
    // uint8_t MOTOR_IN_1_PIN = 0;
    // uint8_t MOTOR_IN_2_PIN = 20;
    // uint8_t WATER_IN_3_PIN = 1;
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
uint32_t* controlTime = new uint32_t(TimeApp::ONE_SECOND * 10); // Интервал проверки сенсоров (по умолчанию 10 секунд)

/** WiFi config */
char SSID[21] = "greenhouse";
char PASSWORD[21] = "";

/** Sensors */
bool* hasSoilMoistureSensor = new bool(false);
bool* hasPhotoSensor = new bool(false);
bool* hasTimeSensor = new bool(false);
bool* hasTemperatureSensor = new bool(false);

/** Temperature config */
float* controlTemperature;

/** Soil moisture config */
uint16_t soilMoistureDryValue = 3750;
uint16_t soilMoistureWetValue = 3600;

/** Time config */
WorkTime* start = new WorkTime(); // Время начала работы аппарата (09:00)
WorkTime* end = new WorkTime(); // Время окончания работы аппарата (20:00)

uint32_t startTimeToInt = start->getHour() * 3600 + start->getMinute() * 60; // Время включения аппарата (09:00)
uint32_t endTimeToInt = end->getHour() * 3600 + end->getMinute() * 60; // Время выключения аппарата (20:00)
