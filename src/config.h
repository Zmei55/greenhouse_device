#pragma once

#include <stdint.h>

#include "enums.h"

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

/** Sensors */
extern bool hasSoilMoistureSensor;
extern bool hasPhotoSensor;
extern bool hasTimeSensor;
extern bool hasTemperatureSensor;

/** Temperature config */
extern float controlTemperature;

/** Soil moisture config */
extern uint16_t soilMoistureDryValue; // Значение, при котором включается полив
extern uint16_t soilMoistureWetValue; // Значение, при котором выключается полив

/** Time config */
extern uint32_t startTimeToInt;
extern uint32_t endTimeToInt;
