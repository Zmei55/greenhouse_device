#include <Arduino.h>
#include <Wire.h>
#include <MyTimer.h>
#include <MyUtils.h>

#include "config.h"
#include "lightSensor.h"
#include "ds3231.h"
#include "api.h"

MyTimer timer;
MyUtils utils;

bool isLedStripsOn; // Включено ли освещение (по ум: false)
bool isWaterOn; // Включен ли полив (по ум: false)
bool isMotorOn; // Включен ли полив (по ум: false)
bool isWindowOpen; // Открыто ли окно (по ум: false)

/** Одноразовая проверка всех датчиков
 * На данный момент не используется, так как будет происходить только при сохранении новых настроек, отправленных с приложения.
 */
void checkSensorsOnce() {
    /** Проверка времени, если время рабочее, то выполняется код */
    bool isWorkingHours = (utils.getNowTimeToInt(rtc.now()) >= startTimeToInt) && (utils.getNowTimeToInt(rtc.now()) <= endTimeToInt);
    
    if (isWorkingHours) {
        /** Если датчик освещенности подключен, то выполняется код... */
        if (hasPhotoSensor) {
            bool isDark = (getLightSensorValue() == true);
            
            /** Если естественного освещения не достаточно, то выполняется код... */
            if (isDark) {
                utils.enablingLighting(&isLedStripsOn);
            }
        }

        /** Если датчик влажности почвы подключен, то выполняется код */
        if (hasSoilMoistureSensor) {
            /** Почва сухая */
            bool isSoilDry = (analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue);

            if (isSoilDry) {
                utils.enablingWatering(&isWaterOn);
            }
        }

        /** Если датчик температуры подключен, и данные с датчика получены, то выполняется код */
        if (hasTemperatureSensor && term.waitReady() && term.readTemp()) {
            /** Текущая температура выше контрольной */
            bool isTemperatureHigh = term.getTemp() > controlTemperature;

            if (isTemperatureHigh && !isWindowOpen) {
                // isMotorOn = true;
                // isWindowOpen = true;
                Serial.print("Жарко!!! ");
                Serial.println(term.getTemp());
                // Serial.print("Мотор: ");
                // Serial.println(isMotorOn ? "вкл" : "выкл");
            }
        }
    }
}

/** Работа всех датчиков по контрольному времени (повторяющийся интервал) */
void checkSensorsByControlTime() {
    timer.interval(controlTime, [](){
        // Serial.println(term.getTemp());
        // Serial.print("Окно: ");
        // Serial.println(isWindowOpen ? "открыто" : "закрыто");

        /** Проверка времени, если время рабочее, то выполняется код */
        bool isWorkingHours = (utils.getNowTimeToInt(rtc.now()) >= startTimeToInt) && (utils.getNowTimeToInt(rtc.now()) <= endTimeToInt);
        if (isWorkingHours) {
            Serial.println("Работаем");
            /** Если датчик освещенности подключен, то выполняется код... */
            if (hasPhotoSensor) {
                bool isDarkAndLedStripsOff = (getLightSensorValue() == true) && (isLedStripsOn == false);
                bool isLightAndLedStripsOn = (getLightSensorValue() == false) && (isLedStripsOn == true);
                
                /** Если естественного освещения не достаточно, то выполняется код... */
                if (isDarkAndLedStripsOff) {
                    utils.enablingLighting(&isLedStripsOn);
                }

                /** Если естественного освещения достаточно, то выполняется код... */
                if (isLightAndLedStripsOn) {
                    utils.disablingLighting(&isLedStripsOn);
                }
            }

            /** Если датчик влажности почвы подключен, то выполняется код */
            if (hasSoilMoistureSensor) {
                /** Почва сухая и полив выключен */
                bool isSoilDryAndWateringOff = (analogRead(SOIL_MOISTURE_PIN) > soilMoistureDryValue) && (isWaterOn == false);

                if (isSoilDryAndWateringOff) {
                    utils.enablingWatering(&isWaterOn);
                }
            }

            /** Если датчик температуры подключен, запрос данных с датчика, ожидание данных, данные с датчика получены, то выполняется код */
            if (hasTemperatureSensor && term.requestTemp() && term.waitReady() && term.readTemp()) {
                /** Текущая температура выше контрольной */
                bool isTemperatureHigh = term.getTemp() > controlTemperature;
                
                if (isTemperatureHigh) {
                    Serial.print("Жарко!!!");
                    Serial.println(term.getTemp());

                    bool isMotorOffAndWindowClosed = (isMotorOn == false) && (isWindowOpen == false);
                    /** Если мотор не включен и окно не открыто, то включаем мотор */
                    if (isMotorOffAndWindowClosed) {
                        isMotorOn = true;
                        Serial.print("Мотор: ");
                        Serial.println(isMotorOn ? "вкл" : "выкл");
                    }
                    // analogWrite(MOTOR_IN_1_PIN, 75);
                    // digitalWrite(MOTOR_IN_1_PIN, HIGH);
                    // digitalWrite(MOTOR_IN_2_PIN, LOW);
                }
            }
        } else {
            /** Если время не рабочее, то выполняется код */
            Serial.println("Не работаем");
        }
    });
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_AP);
    char SSID[21];
    char PASSWORD[21];
    WiFi.softAP(SSID, PASSWORD);

    rtc.begin();
    rtc.adjust(DateTime(2026, 1, 1, 12, 0, 0)); // Установка времени компиляции кода (можно закомментировать после первой загрузки)
    
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    // pinMode(MOTOR_IN_1_PIN, OUTPUT);
    // pinMode(MOTOR_IN_2_PIN, OUTPUT);
    
    term.setResolution(12); // Установка разрешения датчика температуры (9-12 бит, чем выше, тем точнее, но дольше измерение)
    term.requestTemp(); // Запрос на измерение температуры (необходимо для получения данных при первом вызове term.tick())
    isLedStripsOn = false;
    isWaterOn = false;
    isMotorOn = false;
    isWindowOpen = false;

    apiHandler();

    server.begin();
}

void loop() {
    checkSensorsByControlTime();

    /**
     * Проверка уровня влажности почвы каждую секунду после включения полива
     * Если влажность достаточная, то полив отключается, даже если сейчас не рабочее время
     */
    timer.intervalWithFlag(TimeApp::ONE_SECOND, &isWaterOn, [](){
        /** Почва влажная и полив включен */
        bool isSoilWetAndWateringOn = (analogRead(SOIL_MOISTURE_PIN) < soilMoistureWetValue) && (isWaterOn == true);
        
        if (isSoilWetAndWateringOn) {
            utils.disablingWatering(&isWaterOn);
        }
    });

    /** Останавливает мотор через 5 секунд */
    /** После того как flag == true начинается отсчет */
    if (isMotorOn) {
        timer.interval(TimeApp::ONE_SECOND * 5, [](){
            isMotorOn = false;
            Serial.print("Мотор: ");
            Serial.println(isMotorOn ? "вкл" : "выкл");
        });
    }
}

/**
 * TODO: Реализовать управление окном с помощью мотора
 * - при достижении определенной температуры активировать мотор для открытия окна
 *      - V получение данных с датчика температуры
 *      - если температура выше контрольной, то активировать мотор для открытия окна
 *      - V через определенный промежуток времени (например, 10 секунд) остановить мотор,
 *        чтоб процесс открытия окна остановился
 *        (в данный момент процесс синхронный, т.е. код останавливается на время работы мотора, нужно сделать асинхронно)
 * - при достижении определенной температуры активировать мотор для закрытия окна
 *      - V получение данных с датчика температуры
 *      - если температура ниже контрольной, то активировать мотор для закрытия окна
 *      - V через определенный промежуток времени (например, 10 секунд) остановить мотор,
 *        чтоб процесс закрытия окна остановился
 *        (в данный момент процесс синхронный, т.е. код останавливается на время работы мотора, нужно сделать асинхронно)
 * 
 * - таймер для отключения двигателя запускается не корректно,
 *   возможно флаг отрабатывает не верно (исправил, надо проверить)
 */
