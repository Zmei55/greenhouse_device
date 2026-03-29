#include <Arduino.h>
#include <Wire.h>
#include <MyTimer.h>

#include "config.h"
#include "myUtils.h"
#include "ds3231.h"

MyTimer timer;
MyUtils utils;

bool isWaterOn = false;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    rtc.begin();
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Установка времени компиляции кода (можно закомментировать после первой загрузки)
    
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);
}

void loop() {
    /** Одноразовая проверка всех датчиков при включении МК */
    timer.timer(TimeApp::ONE_SECOND, [](){
        utils.lightingPlant();
        utils.enablingWaterByMoisture(&isWaterOn);
    });

    timer.interval(controlTime, [](){
        if ((utils.getNowTimeToInt(rtc.now()) >= startTimeToInt) && (utils.getNowTimeToInt(rtc.now()) <= endTimeToInt)) {
            if (hasPhotoSensor) {
                utils.lightingPlant();
            }

            if (hasSoilMoistureSensor) {
                if ((analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue) && (isWaterOn == false)) {
                    utils.enablingWaterByMoisture(&isWaterOn);
                }
            }
        } else {
            Serial.println("Не работаем");
        }
        // char buf1[] = "hh:mm:ss";
        // Serial.println(now.toString(buf1));
    });

    timer.intervalWithFlag(TimeApp::ONE_SECOND, &isWaterOn, [](){
        if ((analogRead(SOIL_MOISTURE_PIN) < soilMoistureWetValue) && (isWaterOn == true)) {
            utils.disablingWaterByMoisture(&isWaterOn);
        }
    });
}
