#include "MyUtils.h"

MyUtils::MyUtils() {}

void MyUtils::interval(uint32_t delay, void (*callback)()) {
    static uint32_t lastTime = millis();

    if (millis() - lastTime >= delay) {
        lastTime = millis();
        callback();
    }
}

void MyUtils::timer(uint32_t delay, void (*callback)()) {
    static uint32_t lastTime = millis();

    if (millis() - lastTime >= delay) {
        callback();
    }
}

void MyUtils::lightingPlant() {
    static bool ledStripsOn = false;

    /** Включение освещения при темноте */
    if ((getLightSensorValue() == true) && (ledStripsOn == false)) {
        if (ledStripOneEnabled) digitalWrite(LED_STRIP_ONE_PIN, HIGH);
        if (ledStripTwoEnabled) digitalWrite(LED_STRIP_TWO_PIN, HIGH);
        if (ledStripThreeEnabled) digitalWrite(LED_STRIP_THREE_PIN, HIGH);
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
