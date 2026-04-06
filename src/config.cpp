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
uint32_t controlTime = TimeApp::ONE_SECOND * 10;

/** WiFi config */
char SSID[21] = "greenhouse";
char PASSWORD[21] = "";

/** Sensors
 * true - датчик подключен и используется в работе
 * false - датчик не подключен и не используется в работе
 * При отключении датчика, связанный с ним код не выполняется, даже если сейчас рабочее время
 * По умолчанию все датчики отключены и не используются в работе, для исключения ошибок при повторном включении аппарата
 */
bool hasSoilMoistureSensor = true;
// bool hasSoilMoistureSensor = false;
bool hasPhotoSensor = true;
// bool hasPhotoSensor = false;
bool hasTimeSensor = true;
// bool hasTimeSensor = false;
bool hasTemperatureSensor = true;
// bool hasTemperatureSensor = false;

/** Temperature config */
float controlTemperature = 23.0;

/** Soil moisture config */
uint16_t soilMoistureDryValue = 3750;
uint16_t soilMoistureWetValue = 3600;

/** Time config */
WorkTime start(9, 0); // Время начала работы аппарата (09:00)
WorkTime end(20, 0); // Время окончания работы аппарата (20:00)

uint32_t startTimeToInt = start.getHour() * 3600 + start.getMinute() * 60; // Время включения аппарата (09:00)
uint32_t endTimeToInt = end.getMinute() * 3600 + end.getMinute() * 60; // Время выключения аппарата (20:00)
