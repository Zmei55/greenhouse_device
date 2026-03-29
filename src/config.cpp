#include "config.h"

/** Pins */

/** Настройка пинов для ESP32_C3 */
#if defined(ESP32)
// uint8_t MOTOR_IN_1_PIN = 0;
// uint8_t MOTOR_IN_2_PIN = 1;
uint8_t SOIL_MOISTURE_PIN = 3;
uint8_t LED_STRIP_ONE_PIN = 5;
uint8_t LED_STRIP_TWO_PIN = 6;
uint8_t LED_STRIP_THREE_PIN = 7;
uint8_t WATER_PIN = 10;
// uint8_t THERMOMETER_PIN = 20;
uint8_t LIGHT_SENSOR_PIN = 21;
#endif

/** General settings */
uint32_t controlTime = TimeApp::ONE_SECOND;

/** Sensors */
bool hasSoilMoistureSensor = true;
bool hasPhotoSensor = true;
// bool hasPhotoSensor = false;
bool hasTimeSensor = true;

/** Temperature config */
float controlTemperature = 24.0;

/** Soil moisture config */
uint16_t soilMoistureDryValue = 3750;
uint16_t soilMoistureWetValue = 3600;

/** Time config */
struct StartTime {
    uint8_t hour = 9; // Час включения аппарата (09)
    uint8_t minute = 0; // Минуты включения аппарата (00)
} start;
struct EndTime {
    uint8_t hour = 20; // Час выключения аппарата (20)
    uint8_t minute = 0; // Минуты включения аппарата (00)
} end;

uint32_t startTimeToInt = start.hour * 3600 + start.minute * 60; // Время включения аппарата (09:00)
uint32_t endTimeToInt = end.hour * 3600 + end.minute * 60; // Время выключения аппарата (20:00)
