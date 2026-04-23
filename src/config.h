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
extern uint32_t &controlTimeRef; // Интервал проверки показаний датчиков (в миллисекундах), при котором выполняется управление чем-либо (открытие окна, включение полива, включение освещения и т.д.)
extern float &controlTemperatureRef; // Температура, при которой открывается окно (если датчик температуры подключен и используется в работе)
extern uint16_t &soilMoistureDryValueRef; // Значение датчика влажности почвы, при котором включается полив
extern uint16_t &soilMoistureWetValueRef; // Значение датчика влажности почвы, при котором выключается полив
extern uint32_t &runningWindowMotorTimeRef; // Время, в течение которого мотор открывает/закрывает окно (в секундах)
extern bool &isLedStripsOnRef; // Включено ли освещение (по ум: false)
extern bool &isWaterOnRef; // Включен ли полив (по ум: false)
extern bool &isMotorOnRef; // Включен ли полив (по ум: false)
extern bool &isWindowOpenRef; // Открыто ли окно (по ум: false)

/** WiFi config */
extern char SSID[21];
extern char PASSWORD[21];

/** Sensors
 * true - датчик подключен и используется в работе
 * false - датчик не подключен и не используется в работе
 * При отключении датчика, связанный с ним код не выполняется, даже если сейчас рабочее время
 * По умолчанию все датчики отключены и не используются в работе, для исключения ошибок при повторном включении аппарата
 */
extern bool &hasSoilMoistureSensorRef; // Датчик влажности почвы
extern bool &hasPhotoSensorRef; // Датчик освещённости
extern bool &hasTemperatureSensorRef;  // Датчик температуры

/** Time config */
extern bool &isWorkTimeEnabledRef; // Включено ли рабочее время
extern WorkTime &WTStartRef; // Время начала работы аппарата
extern WorkTime &WTEndRef; // Время окончания работы аппарата
