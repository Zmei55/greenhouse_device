#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "lightSensor.h"
#include "dht22.h"
#include "myUtils.h"

MyUtils utils;

bool redLightOn = false;
bool ledStripsOn = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    if (hasDHT22Sensor) dht.begin();
}

void loop() {
    utils.interval(controlTime, [](){
        if (hasPhotoSensor) {
            /** Включение освещения при темноте */
            if ((getLightSensorValue() == true) && (ledStripsOn == false)) {
                if (ledStripOneEnabled) digitalWrite(LED_STRIP_ONE_PIN, HIGH);
                if (ledStripTwoEnabled) digitalWrite(LED_STRIP_TWO_PIN, HIGH);
                if (ledStripThreeEnabled) digitalWrite(LED_STRIP_THREE_PIN, HIGH);
                ledStripsOn = true;
            }
            /** Выключение освещения при свете */
            if ((getLightSensorValue() == false) && (ledStripsOn == true)) {
                digitalWrite(LED_STRIP_ONE_PIN, LOW);
                digitalWrite(LED_STRIP_TWO_PIN, LOW);
                digitalWrite(LED_STRIP_THREE_PIN, LOW);
                ledStripsOn = false;
            }
        }

        if (hasDHT22Sensor) {
            /** Включение ... если температура выше установленного значения */
            if ((getTemperature() > controlTemperature) && (redLightOn == false)) {
                digitalWrite(RED_LED_PIN, HIGH);
                redLightOn = true;
            }
            /** Выключение ... если температура ниже установленного значения */
            if ((getTemperature() < controlTemperature) && (redLightOn == true)) {
                digitalWrite(RED_LED_PIN, LOW);
                redLightOn = false;
            }
        }

        if (hasSoilMoistureSensor) {
            Serial.println(analogRead(SOIL_MOISTURE_PIN));
        }
    });
}
