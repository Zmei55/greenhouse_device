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

void MyUtils::controlMotorByTemperature() {
    static bool redLightOn = false;

    /** Включение ... если температура выше установленного значения */
    if ((getTemperature() > controlTemperature) && (redLightOn == false)) {
        digitalWrite(RED_LED_PIN, HIGH);
        redLightOn = true;
    }
    /** Выключение ... если температура ниже установленного значения */
    if ((getTemperature() < controlTemperature) && (redLightOn == true)) {
        digitalWrite(RED_LED_PIN, LOW);
        redLightOn = false;
    }
}

void MyUtils::enablingWaterByMoisture(bool *flag) {
    /** Включение полива при сухой почве */
    if ((analogRead(SOIL_MOISTURE_PIN) > 3750) && (*flag == false)) {
        digitalWrite(WATER_PIN, HIGH);
        *flag = true;
    }
}
