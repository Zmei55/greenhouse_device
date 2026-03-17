#include "config.h"

/** Pins */
uint8_t RED_LED_PIN = 0;
uint8_t LED_STRIP_ONE_PIN = 5;
uint8_t LED_STRIP_TWO_PIN = 6;
uint8_t LED_STRIP_THREE_PIN = 7;
uint8_t SOIL_MOISTURE_PIN = 1;
uint8_t WATER_PIN = 10;
uint8_t DHT22_PIN = 20;
uint8_t LIGHT_SENSOR_PIN = 21;

/** General settings */
uint32_t controlTime = TimeApp::ONE_SECOND;

/** Sensors */
bool hasDHT22Sensor = true;
bool hasSoilMoistureSensor = true;
bool hasPhotoSensor = true;
// bool hasTimeSensor = true;

/** DHT config */
float controlTemperature = 24.0;

/** Led strips config */
bool ledStripOneEnabled = true;
bool ledStripTwoEnabled = true;
bool ledStripThreeEnabled = true;
