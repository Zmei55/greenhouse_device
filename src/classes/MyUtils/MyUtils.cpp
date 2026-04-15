#include "MyUtils.h"

#include <Arduino.h>

#include "config.h"

MyUtils::MyUtils() {}

uint32_t MyUtils::getNowTimeToInt(const DateTime& time) {
    return time.hour() * 3600 + time.minute() * 60;
}

void MyUtils::enablingLighting(bool *flag) {
    digitalWrite(LED_STRIP_ONE_PIN, HIGH);
    digitalWrite(LED_STRIP_TWO_PIN, HIGH);
    digitalWrite(LED_STRIP_THREE_PIN, HIGH);
    *flag = true;
}

void MyUtils::disablingLighting(bool *flag) {
    digitalWrite(LED_STRIP_ONE_PIN, LOW);
    digitalWrite(LED_STRIP_TWO_PIN, LOW);
    digitalWrite(LED_STRIP_THREE_PIN, LOW);
    *flag = false;
}

void MyUtils::enablingWatering(bool *flag) {
    // digitalWrite(WATER_PIN, HIGH);
    *flag = true;
}

void MyUtils::disablingWatering(bool *flag) {
    // digitalWrite(WATER_PIN, LOW);
    *flag = false;
}

void MyUtils::openingWindow(bool *flag) {};

void MyUtils::closingWindow(bool *flag) {};
