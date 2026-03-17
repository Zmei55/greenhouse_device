#pragma once

#include <Arduino.h>

#include "config.h"
#include "lightSensor.h"

class MyUtils {
   public:
    MyUtils();

    /** Срабатывает много раз по достижении задержки (delay) */
    void interval(uint32_t delay, void (*callback)());

    /** Срабатывает один раз по достижении задержки (delay) */
    void timer(uint32_t delay, void (*callback)());

    /** Управляет освещением растений */
    void lightingPlant();
};
