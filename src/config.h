#pragma once

#include <stdint.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <GyverDS18.h>
#include <WorkTime.h>

#include "enums.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif


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
extern bool *hasTimeSensor; // Датчик реального времени
extern bool *hasTemperatureSensor;  // Датчик температуры

/** Time config */
extern WorkTime *start; // Время начала работы аппарата
extern WorkTime *end; // Время окончания работы аппарата
extern uint32_t startTimeToInt; // Время начала работы аппарата в виде целого числа (секунды с 00:00:00)
extern uint32_t endTimeToInt; // Время окончания работы аппарата в виде целого числа (секунды с 00:00:00)
