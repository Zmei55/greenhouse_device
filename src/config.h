#pragma once

/** Pins */
#define RED_LED_PIN 0
#define WHITE_LED_1_PIN 5
#define WHITE_LED_2_PIN 6
#define WHITE_LED_3_PIN 7
#define SOIL_MOISTURE_PIN 1

/** Sensors */
bool hasDHT22Sensor = true;
// bool hasSoilMoistureSensor = false;
bool hasPhotoSensor = true;
// bool hasTimeSensor = true;

/** DHT config */
float controlTemperature = 24.0;
