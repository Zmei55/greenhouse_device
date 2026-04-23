#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "./sensors/lightSensor.h"
#include "./sensors/ds3231.h"
#include "api.h"

void checkSensors();

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
    
    term.setResolution(12); // Установка разрешения датчика температуры (9-12 бит, чем выше, тем точнее, но дольше измерение)
    term.requestTemp(); // Запрос на измерение температуры (необходимо для получения данных при первом вызове term.tick())

    apiHandler();
    server.begin();
}

void loop() {
    /** Работа всех датчиков по контрольному времени (повторяющийся интервал) */
    timer.interval(*controlTime, [](){
        if (*isWorkTimeEnabled) {
            // код выполняется, если рабочее время установлено
            /** Проверка времени, если время рабочее, то выполняется код */
            bool isWorkingHours = 
                (utils.getNowTimeToInt(rtc.now()) >= utils.convertWorkTimeToInt(start->getHour(), start->getMinute())) && 
                (utils.getNowTimeToInt(rtc.now()) <= utils.convertWorkTimeToInt(end->getHour(), end->getMinute()))
            ;
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
}

/** Выполняется проверка всех датчиков и выполнение соответствующих инструкций */
void checkSensors() {
    /** Если датчик освещенности подключен, то выполняется код... */
    if (*hasPhotoSensor) {
        bool isDarkAndLedStripsOff = (getLightSensorValue() == true) && (*isLedStripsOn == false); // темно и освещение выключено
        bool isLightAndLedStripsOn = (getLightSensorValue() == false) && (*isLedStripsOn == true); // светло и освещение включено
        
        /** Если естественного освещения не достаточно, то выполняется код... */
        if (isDarkAndLedStripsOff) {
            utils.enablingLighting(isLedStripsOn);
        }

        /** Если естественного освещения достаточно, то выполняется код... */
        if (isLightAndLedStripsOn) {
            utils.disablingLighting(isLedStripsOn);
        }
    }
}
