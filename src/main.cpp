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
    // pinMode(RED_LED_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    // dht.begin();
}

void loop() {
    /** Одноразовая проверка всех датчиков при включении МК */
    timer.timer(TimeApp::ONE_SECOND, [](){
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }
        // if (hasDHT22Sensor) {
        //     utils.controlMotorByTemperature(&isWindowMotorEnabled);
        // }
        if (hasSoilMoistureSensor) {
            utils.enablingWaterByMoisture(&isWaterOn);
        }
    });

    timer.interval(controlTime, [](){
        Serial.println(getTemperature());
        Serial.print("motor enabled: ");
        Serial.println(isWindowMotorEnabled ? "true" : "false");
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }

        // if (hasDHT22Sensor) {
        //     utils.controlMotorByTemperature(&isWindowMotorEnabled);
        // }

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

    /** Останавливает мотор через 5 секунд */
    /** После того как flag == true начинается отсчет */
    // timer.timerWithFlag(TimeApp::ONE_SECOND * 5, &isWindowMotorEnabled, [](){
    //     utils.disabledWindowMotor(&isWindowMotorEnabled);
    // });
}

/**
 * TODO: Реализовать управление окном с помощью мотора
 * - при достижении определенной температуры актевировать мотор для открытия окна
 *      - через определенный промежуток времени (например, 10 секунд) остановить мотор,
 *        чтоб процесс открытия окна остановился
 * - при достижении определенной температуры активировать мотор для закрытия окна
 *     - через определенный промежуток времени (например, 10 секунд) остановить мотор,
 *       чтоб процесс закрытия окна остановился
 * 
 * - таймер для отключения двигателя запускается не корректно,
 *   возможно флаг отрабатывает не верно (исправил, надо проверить)
 */
