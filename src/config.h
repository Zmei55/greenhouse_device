#pragma once

#include "enums.h"

/** Pins */
#define RED_LED_PIN 0
#define LED_STRIP_ONE_PIN 5
#define LED_STRIP_TWO_PIN 6
#define LED_STRIP_THREE_PIN 7
#define SOIL_MOISTURE_PIN 1

/** General settings */
unsigned long controlTime = TimeApp::ONE_SECOND;

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
