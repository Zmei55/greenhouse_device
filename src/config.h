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
extern uint32_t controlTime;

/** WiFi config */
extern char SSID[21];
extern char PASSWORD[21];

/** Sensors
 * true - датчик подключен и используется в работе
 * false - датчик не подключен и не используется в работе
 * При отключении датчика, связанный с ним код не выполняется, даже если сейчас рабочее время
 * По умолчанию все датчики отключены и не используются в работе, для исключения ошибок при повторном включении аппарата
 */
extern bool* hasSoilMoistureSensor;
extern bool* hasPhotoSensor;
extern bool* hasTimeSensor;
extern bool* hasTemperatureSensor;

/** Temperature config */
extern float controlTemperature;

/** Soil moisture config */
extern uint16_t soilMoistureDryValue; // Значение, при котором включается полив
extern uint16_t soilMoistureWetValue; // Значение, при котором выключается полив

/** Time config */
extern WorkTime* start;
extern WorkTime* end;
extern uint32_t startTimeToInt;
extern uint32_t endTimeToInt;
