#pragma once

#include <Arduino.h>
#include "config.h"

#define LIGHT_SENSOR_PIN 21

bool isDark = false;

/** Возвращает булевое значение освещенности
 * настраивается на датчике, переменным резистором
 */
bool getLightSensorValue() {
    isDark = digitalRead(LIGHT_SENSOR_PIN);
    return isDark;
}
