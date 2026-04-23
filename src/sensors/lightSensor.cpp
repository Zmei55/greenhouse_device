#include "lightSensor.h"

#include <Arduino.h>

bool getLightSensorValue() {
    static bool isDark = false;

    isDark = digitalRead(LIGHT_SENSOR_PIN);
    return isDark;
}
