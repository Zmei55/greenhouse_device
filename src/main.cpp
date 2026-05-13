#include <Arduino.h>
#include <Wire.h>

#include "./sensors/thermometer.h"
#include "api.h"
#include "config.h"

void checkSensors();
void stopingWindowMotor(const uint32_t &startMotor, const uint32_t &delay);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);

    rtc.begin();
    rtc.adjust(DateTime(2026, 1, 1, 12, 0, 0)); // Установка времени устройства по умолчанию

    pinMode(lighting.getPhotoSensorPin(), INPUT);
    pinMode(lighting.getLedStripOnePin(), OUTPUT);
    pinMode(lighting.getLedStripTwoPin(), OUTPUT);
    pinMode(lighting.getLedStripThreePin(), OUTPUT);
    pinMode(watering.getMoisturePin(), INPUT);
    pinMode(watering.getPumpOnePin(), OUTPUT);
    pinMode(watering.getPumpTwoPin(), OUTPUT);
    pinMode(THERMOMETER_PIN, INPUT);
    pinMode(window.getMotorPinOne(), OUTPUT);
    pinMode(window.getMotorPinTwo(), OUTPUT);

    term.setResolution(12); // Установка разрешения датчика температуры (9-12 бит, чем выше, тем точнее, но дольше измерение)
    term.requestTemp(); // Запрос на измерение температуры (необходимо для получения данных при первом вызове term.tick())

    apiHandler();
    server.begin();
}

void loop() {
    /** Если рабочее время установлено... */
    if (isWorkTimeEnabledRef) {
        /** Работа всех датчиков по контрольному времени (повторяющийся интервал) */
        timer.interval(controlTimeRef, []() {
            // код выполняется, если рабочее время установлено
            bool isWorkingTime = (utils.getNowTimeToInt(rtc.now()) >= WTStartRef.getWorkTimeAsInt()) &&
                                 (utils.getNowTimeToInt(rtc.now()) <= WTEndRef.getWorkTimeAsInt()); // Рабочее ли время

            if (isWorkingTime) {
                // код выполняется, если время рабочее
                checkSensors();
            }
        });

        /** Если осталось 15 мин до окончания рабочего времени */
        if ((utils.getNowTimeToInt(rtc.now()) >= (WTEndRef.getWorkTimeAsInt() - 1000 * 60 * 25)) && (utils.getNowTimeToInt(rtc.now()) < (WTEndRef.getWorkTimeAsInt() - 1000 * 60 * 2))) {
            timer.interval(1000 * 60 * 15, []() {
                checkSensors();
            });
        }

        /** Если осталась 1 мин до окончания рабочего времени */
        if ((utils.getNowTimeToInt(rtc.now()) >= (WTEndRef.getWorkTimeAsInt() - 1000 * 60 * 2)) && !window.getIsMotorOn() && window.getIsWindowOpen()) {
            timer.interval(1000 * 60, []() {
                window.close();
            });
        }
    }

    /**
     * Если мотор, открывающий/закрывающий окно, включен, то выполняется код...
     * даже если сейчас не рабочее время
     */
    if (window.getIsMotorOn())
        stopingWindowMotor(window.getMotorStartTime(), window.getRunningMotorTime());

    /**
     * Проверка уровня влажности почвы каждую секунду после включения полива
     * Если влажность достаточная, то полив отключается, даже если сейчас не рабочее время
     */
    timer.intervalWithFlag(1000, watering.isPumpOn(), []() {
        bool isSoilDryAndWateringOn = watering.isSoilDry() && watering.isPumpOn(); // Почва влажная и полив включен

        if (isSoilDryAndWateringOn) {
            watering.disable();
        }
    });
}

/** Выполняется проверка всех датчиков и выполнение соответствующих инструкций */
void checkSensors() {
    /** Если датчик освещенности подключен, то выполняется код... */
    if (hasPhotoSensorRef) {
        bool isDarkAndLedStripsOff = lighting.isDark() && !lighting.isLedStripsOn(); // темно и освещение выключено
        bool isLightAndLedStripsOn = !lighting.isDark() && lighting.isLedStripsOn(); // светло и освещение включено

        /** Если естественного освещения не достаточно, то выполняется код... */
        if (isDarkAndLedStripsOff) {
            lighting.on();
        }

        /** Если естественного освещения достаточно, то выполняется код... */
        if (isLightAndLedStripsOn) {
            lighting.off();
        }
    }

    /** Если датчик температуры подключен, запрос данных с датчика, ожидание данных, данные с датчика получены, то выполняется код... */
    if (hasTemperatureSensorRef && term.requestTemp() && term.waitReady() && term.readTemp()) {
        Serial.print("Температура: ");
        Serial.println(term.getTemp());
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

    /** Если датчик влажности почвы подключен, то выполняется код... */
    if (hasSoilMoistureSensorRef) {
        Serial.print("Влажность почвы: ");
        Serial.println(analogRead(SOIL_MOISTURE_PIN));
        bool isSoilWetAndWateringOff = watering.isSoilWet() && !watering.isPumpOn(); // Почва сухая и полив выключен

        if (isSoilWetAndWateringOff) {
            watering.enable();
        }
    }
}

/**
 * @brief Выполняется остановка мотора, открывающего/закрывающего окно
 * @param startMotor начало работы мотора
 * @param delay время, через которое мотор должен остановиться
 */
void stopingWindowMotor(const uint32_t &startMotor, const uint32_t &delay) {
    if (millis() - startMotor >= delay) {
        window.stopMotor();
        window.toggleWindowState();
        window.resetMotorStartTime();
    }
}
