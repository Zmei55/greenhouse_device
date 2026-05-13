#include "config.h"

#if defined(ESP32) || defined(ESP8266)
AsyncWebServer server(80);
#endif

/** Объявление классов */
MyTimer timer;
MyUtils utils;
Window window(MOTOR_IN_1_PIN, MOTOR_IN_2_PIN);
Lighting lighting(LIGHT_SENSOR_PIN, LED_STRIP_ONE_PIN, LED_STRIP_TWO_PIN, LED_STRIP_THREE_PIN);
Watering watering(SOIL_MOISTURE_PIN, PUMPE_IN_1_PIN, PUMPE_IN_2_PIN);

/** General settings */
uint32_t controlTime = 1000 * 60 * 60; // Проверка датчиков происходит каждый час
float controlTemperature = 24.0;

uint32_t &controlTimeRef = controlTime;
float &controlTemperatureRef = controlTemperature;

/** WiFi config */
char SSID[21] = "greenhouse";
char PASSWORD[21] = "";

/** Sensors */
bool hasSoilMoistureSensor = true; // Датчик влажности почвы подключен
bool hasPhotoSensor = false;
bool hasTemperatureSensor = true; // Датчик температуры подключен

bool &hasSoilMoistureSensorRef = hasSoilMoistureSensor;
bool &hasPhotoSensorRef = hasPhotoSensor;
bool &hasTemperatureSensorRef = hasTemperatureSensor;

/** Time config */
bool isWorkTimeEnabled = true; // Рабочее время установлено
WorkTime start = WorkTime(8, 0);
WorkTime end = WorkTime(20, 0);

bool &isWorkTimeEnabledRef = isWorkTimeEnabled;
WorkTime &WTStartRef = start;
WorkTime &WTEndRef = end;
