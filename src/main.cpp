#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "./sensors/lightSensor.h"
#include "./sensors/thermometer.h"
#include "api.h"

void checkSensors();
void stopingWindowMotor(uint32_t startMotor, uint32_t delay);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);

    rtc.begin();
    rtc.adjust(DateTime(2026, 1, 1, 12, 0, 0)); // Установка времени устройства по умолчанию
    
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(THERMOMETER_PIN, INPUT);
    pinMode(window.getMotorPinOne(), OUTPUT);
    pinMode(window.getMotorPinTwo(), OUTPUT);
    
    term.setResolution(12); // Установка разрешения датчика температуры (9-12 бит, чем выше, тем точнее, но дольше измерение)
    term.requestTemp(); // Запрос на измерение температуры (необходимо для получения данных при первом вызове term.tick())

    apiHandler();
    server.begin();
}

void loop() {
    /** Работа всех датчиков по контрольному времени (повторяющийся интервал) */
    timer.interval(controlTimeRef, [](){
        if (isWorkTimeEnabledRef) {
            // код выполняется, если рабочее время установлено
            /** Проверка времени, если время рабочее, то выполняется код */
            bool isWorkingHours = (utils.getNowTimeToInt(rtc.now()) >= WTStartRef.getWorkTimeAsInt()) && (utils.getNowTimeToInt(rtc.now()) <= WTEndRef.getWorkTimeAsInt());
            if (isWorkingHours) {
                // код выполняется, если время рабочее
                checkSensors();
            } else {
                // код выполняется, если время не рабочее
            }
        }  else {
            // код выполняется, если рабочее время не установлено
            checkSensors();
        }
    });

    /** Если мотор, открывающий/закрывающий окно, включен, то выполняется код... */
    if (window.getIsMotorOn()) stopingWindowMotor(window.getMotorStartTime(), window.getRunningMotorTime());
}

/** Выполняется проверка всех датчиков и выполнение соответствующих инструкций */
void checkSensors() {
    /** Если датчик освещенности подключен, то выполняется код... */
    if (hasPhotoSensorRef) {
        bool isDarkAndLedStripsOff = (getLightSensorValue() == true) && (isLedStripsOnRef == false); // темно и освещение выключено
        bool isLightAndLedStripsOn = (getLightSensorValue() == false) && (isLedStripsOnRef == true); // светло и освещение включено
        
        /** Если естественного освещения не достаточно, то выполняется код... */
        if (isDarkAndLedStripsOff) {
            utils.enablingLighting(isLedStripsOnRef);
        }

        /** Если естественного освещения достаточно, то выполняется код... */
        if (isLightAndLedStripsOn) {
            utils.disablingLighting(isLedStripsOnRef);
        }
    }

    /** Если датчик температуры подключен, запрос данных с датчика, ожидание данных, данные с датчика получены, то выполняется код... */
    if (hasTemperatureSensorRef && term.requestTemp() && term.waitReady() && term.readTemp()) {
        bool isTemperatureHigh = term.getTemp() > controlTemperatureRef; // Текущая температура выше контрольной

        /** Если температура выше контрольной, мотор выключен, окно закрыто, то открывает окно */
        if (isTemperatureHigh && !window.getIsMotorOn() && !window.getIsWindowOpen()) {
            window.open();
        }

        /** Если температура ниже контрольной, мотор выключен, окно открыто, то закрывает окно */
        if (!isTemperatureHigh && !window.getIsMotorOn() && window.getIsWindowOpen()) {
            window.close();
        }
    }
}

/**
 * Выполняется остановка мотора, открывающего/закрывающего окно
 * @param startMotor начало работы мотора
 * @param delay время, через которое мотор должен остановиться
 */
void stopingWindowMotor(uint32_t startMotor, uint32_t delay) {
    if (millis() - startMotor >= delay) {
        window.stopMotor();
        window.toggleWindowState();
        window.resetMotorStartTime();
    }
}
