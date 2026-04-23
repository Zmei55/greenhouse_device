#pragma once

#include <stdint.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <GyverDS18.h>
#include <MyTimer.h>

#include "enums.h"
#include "classes/MyUtils/MyUtils.h"
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
extern GyverDS18Single term;

/** Pins */
// extern uint8_t MOTOR_IN_1_PIN;
// extern uint8_t MOTOR_IN_2_PIN;
// extern uint8_t WATER_IN_3_PIN;
// extern uint8_t WATER_IN_4_PIN;
extern uint8_t LED_STRIP_ONE_PIN;
extern uint8_t LED_STRIP_TWO_PIN;
extern uint8_t LED_STRIP_THREE_PIN;
extern uint8_t SOIL_MOISTURE_PIN;
extern uint8_t THERMOMETER_PIN;
extern uint8_t LIGHT_SENSOR_PIN;

/** General settings */
extern uint32_t *controlTime; // Интервал проверки показаний датчиков (в миллисекундах), при котором выполняется управление чем-либо (открытие окна, включение полива, включение освещения и т.д.)
extern float *controlTemperature; // Температура, при которой открывается окно (если датчик температуры подключен и используется в работе)
extern uint16_t soilMoistureDryValue; // Значение датчика влажности почвы, при котором включается полив
extern uint16_t soilMoistureWetValue; // Значение датчика влажности почвы, при котором выключается полив
extern uint32_t *runningWindowMotorTime; // Время, в течение которого мотор открывает/закрывает окно (в секундах)
extern bool *isLedStripsOn; // Включено ли освещение (по ум: false)
extern bool *isWaterOn; // Включен ли полив (по ум: false)
extern bool *isMotorOn; // Включен ли полив (по ум: false)
extern bool *isWindowOpen; // Открыто ли окно (по ум: false)

/** WiFi config */
extern char SSID[21];
extern char PASSWORD[21];

/** Sensors
 * true - датчик подключен и используется в работе
 * false - датчик не подключен и не используется в работе
 * При отключении датчика, связанный с ним код не выполняется, даже если сейчас рабочее время
 * По умолчанию все датчики отключены и не используются в работе, для исключения ошибок при повторном включении аппарата
 */
extern bool *hasSoilMoistureSensor; // Датчик влажности почвы
extern bool *hasPhotoSensor; // Датчик освещённости
extern bool *hasTemperatureSensor;  // Датчик температуры

/** Time config */
extern bool *isWorkTimeEnabled; // Включено ли рабочее время
extern WorkTime *start; // Время начала работы аппарата
extern WorkTime *end; // Время окончания работы аппарата
