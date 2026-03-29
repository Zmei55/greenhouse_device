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
    
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(WATER_PIN, OUTPUT);
}

void loop() {
    /** Одноразовая проверка всех датчиков при включении МК */
    timer.timer(TimeApp::ONE_SECOND, [](){
        /** Если датчик освещенности подключен, то выполняется код */
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }
        /** Если датчик влажности почвы подключен, то выполняется код */
        if (hasSoilMoistureSensor) {
            if ((analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue) && (isWaterOn == false)) {
                utils.enablingWaterByMoisture(&isWaterOn);
            }
        }
    });

    timer.interval(controlTime, [](){
        /** Проверка времени, если время рабочее, то выполняется код */
        if ((utils.getNowTimeToInt(rtc.now()) >= startTimeToInt) && (utils.getNowTimeToInt(rtc.now()) <= endTimeToInt)) {
            /** Если датчик освещенности подключен, то выполняется код */
            if (hasPhotoSensor) {
                utils.lightingPlant();
            }

            /** Если датчик влажности почвы подключен, то выполняется код */
            if (hasSoilMoistureSensor) {
                if ((analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue) && (isWaterOn == false)) {
                    utils.enablingWaterByMoisture(&isWaterOn);
                }
            }
        } else {
            /** Если время не рабочее, то выполняется код */
            Serial.println("Не работаем");
        }
    });

    /**
     * Проверка уровня влажности почвы каждую секунду
     * Если влажность достаточная, то полив отключается, даже если сейчас рабочее время
     */
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
