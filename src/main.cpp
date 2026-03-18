#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "dht22.h"
#include "myUtils.h"

MyUtils utils;

void setup() {
    Serial.begin(115200);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    if (hasDHT22Sensor) dht.begin();
}

void loop() {
    utils.interval(controlTime, [](){
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }

        if (hasDHT22Sensor) {
            utils.controlMotorByTemperature();
        }

        if (hasSoilMoistureSensor) {
            utils.controlWaterByMoisture();
        }
    });
}
