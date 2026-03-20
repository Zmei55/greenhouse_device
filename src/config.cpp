#include "config.h"

/** Pins */

/** Настройка пинов для ESP32_C3 */
#if defined(ESP32)
uint8_t RED_LED_PIN = 0;
uint8_t SOIL_MOISTURE_PIN = 1;
uint8_t LED_STRIP_ONE_PIN = 5;
uint8_t LED_STRIP_TWO_PIN = 6;
uint8_t LED_STRIP_THREE_PIN = 7;
uint8_t WATER_PIN = 10;
uint8_t DHT22_PIN = 20;
uint8_t LIGHT_SENSOR_PIN = 21;
#endif

/** General settings */
uint32_t controlTime = TimeApp::ONE_SECOND * 10;

/** Sensors */
bool hasDHT22Sensor = true;
// bool hasDHT22Sensor = false;
bool hasSoilMoistureSensor = true;
bool hasPhotoSensor = true;
// bool hasPhotoSensor = false;
// bool hasTimeSensor = true;

/** DHT config */
float controlTemperature = 24.0;

/** Soil moisture config */
uint16_t soilMoistureDryValue = 3750;
uint16_t soilMoistureWetValue = 3600;
