#include "MyUtils.h"

#include <Arduino.h>

#include "config.h"
#include "lightSensor.h"
#include "dht22.h"

MyUtils::MyUtils() {}

void MyUtils::lightingPlant() {
    static bool ledStripsOn = false;

    /** Включение освещения при темноте */
    if ((getLightSensorValue() == true) && (ledStripsOn == false)) {
        digitalWrite(LED_STRIP_ONE_PIN, HIGH);
        digitalWrite(LED_STRIP_TWO_PIN, HIGH);
        digitalWrite(LED_STRIP_THREE_PIN, HIGH);
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

void MyUtils::enablingWaterByMoisture(bool *flag) {
    digitalWrite(WATER_PIN, HIGH);
    *flag = true;
}

void MyUtils::disablingWaterByMoisture(bool *flag) {
    digitalWrite(WATER_PIN, LOW);
    *flag = false;
}

/** Запуск мотора для открытия окна */
// void openingWindow() {
//     digitalWrite(RED_LED_PIN, HIGH);
// }

/** Запуск мотора для закрытия окна */
// void closingWindow() {
//     digitalWrite(RED_LED_PIN, HIGH);
// }

// void MyUtils::controlMotorByTemperature(bool *flag) {
//     static bool isWindowOpen = false;

//     /** Открывает окно если температура выше установленного значения */
//     if ((getTemperature() > controlTemperature) && (isWindowOpen == false)) {
//         openingWindow();
//         *flag = true;
//         isWindowOpen = true;
//     }
//     /** Закрывает окно если температура ниже установленного значения */
//     if ((getTemperature() < controlTemperature) && (isWindowOpen == true)) {
//         closingWindow();
//         *flag = true;
//         isWindowOpen = false;
//     }
// }

// void MyUtils::disabledWindowMotor(bool *flag) {
//     digitalWrite(RED_LED_PIN, LOW);
//     *flag = false;
// }
