#include <Arduino.h>
#include <Wire.h>
#include <MyTimer.h>

#include "config.h"
#include "dht22.h"
#include "myUtils.h"

MyTimer timer;
MyUtils utils;

bool isWaterOn = false;
bool isWindowMotorEnabled = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    dht.begin();
}

void loop() {
    /** Одноразовая проверка всех датчиков при включении МК */
    timer.timer(TimeApp::ONE_SECOND, [](){
        utils.lightingPlant();
        utils.controlMotorByTemperature(&isWindowMotorEnabled);
        utils.enablingWaterByMoisture(&isWaterOn);
    });

    timer.interval(controlTime, [](){
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }

        if (hasDHT22Sensor) {
            utils.controlMotorByTemperature(&isWindowMotorEnabled);
        }

        if (hasSoilMoistureSensor) {
            if ((analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue) && (isWaterOn == false)) {
                utils.enablingWaterByMoisture(&isWaterOn);
            }
        }
    });

    timer.intervalWithFlag(TimeApp::ONE_SECOND, &isWaterOn, [](){
        if ((analogRead(SOIL_MOISTURE_PIN) < soilMoistureWetValue) && (isWaterOn == true)) {
            utils.disablingWaterByMoisture(&isWaterOn);
        }
    });
}
