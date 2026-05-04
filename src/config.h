#pragma once

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <MyTimer.h>
#include <stdint.h>

#include "classes/Lighting/Lighting.h"
#include "classes/MyUtils/MyUtils.h"
#include "classes/Watering/Watering.h"
#include "classes/Window/Window.h"
#include "classes/WorkTime/WorkTime.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

extern MyTimer timer;
extern MyUtils utils;
extern AsyncWebServer server;
extern Window window;
extern Lighting lighting;
extern Watering watering;

/** Pins */

/** Настройка пинов для ESP32_C3 */
#if defined(ESP32)
#define MOTOR_IN_1_PIN 0
#define MOTOR_IN_2_PIN 1
#define PUMPE_IN_1_PIN 20
#define PUMPE_IN_2_PIN 10
#define LED_STRIP_ONE_PIN 5
#define LED_STRIP_TWO_PIN 6
#define LED_STRIP_THREE_PIN 7
#define SOIL_MOISTURE_PIN 4
#define THERMOMETER_PIN 3
#define LIGHT_SENSOR_PIN 21
#endif

// #if defined(ESP8266)
// здесь можно указать пины для ESP8266, если планируется поддержка этой платформы
// #endif

/** General settings */
extern uint32_t &controlTimeRef; // Интервал проверки показаний датчиков (в миллисекундах), при котором выполняется управление чем-либо (открытие окна, включение полива, включение освещения и т.д.)
extern float &controlTemperatureRef; // Температура, при которой открывается окно (если датчик температуры подключен и используется в работе)

/** WiFi config */
extern char SSID[21];
extern char PASSWORD[21];

/** Sensors
 * true - датчик подключен и используется в работе
 * false - датчик не подключен и не используется в работе
 * При отключении датчика, связанный с ним код не выполняется, даже если сейчас рабочее время
 * По умолчанию все датчики отключены и не используются в работе, для исключения ошибок при
 * повторном включении аппарата
 */
extern bool &hasSoilMoistureSensorRef; // Датчик влажности почвы
extern bool &hasPhotoSensorRef; // Датчик освещённости
extern bool &hasTemperatureSensorRef; // Датчик температуры

/** Time config */
extern bool &isWorkTimeEnabledRef; // Включено ли рабочее время
extern WorkTime &WTStartRef; // Время начала работы аппарата
extern WorkTime &WTEndRef; // Время окончания работы аппарата
