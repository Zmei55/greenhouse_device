#pragma once

#include <stdint.h>

#include "enums.h"

/** Pins */
extern uint8_t RED_LED_PIN;
extern uint8_t LED_STRIP_ONE_PIN;
extern uint8_t LED_STRIP_TWO_PIN;
extern uint8_t LED_STRIP_THREE_PIN;
extern uint8_t SOIL_MOISTURE_PIN;
extern uint8_t WATER_PIN;
extern uint8_t DHT22_PIN;
extern uint8_t LIGHT_SENSOR_PIN;

/** General settings */
extern uint32_t controlTime;

/** Sensors */
extern bool hasDHT22Sensor;
extern bool hasSoilMoistureSensor;
extern bool hasPhotoSensor;
// extern bool hasTimeSensor;

/** DHT config */
extern float controlTemperature;

/** Led strips config */
extern bool ledStripOneEnabled;
extern bool ledStripTwoEnabled;
extern bool ledStripThreeEnabled;
