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
