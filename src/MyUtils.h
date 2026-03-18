#pragma once

#include <stdint.h>

class MyUtils {
   public:
    MyUtils();

    /** Срабатывает много раз по достижении задержки (delay) */
    void interval(uint32_t delay, void (*callback)());

    /** Срабатывает один раз по достижении задержки (delay) */
    void timer(uint32_t delay, void (*callback)());

    /** Управляет освещением растений */
    void lightingPlant();

    /** Управление открыванием окна в зависимости от температуры */
    void controlMotorByTemperature();

    /** Управление поливом в зависимости от влажности почвы */
    void controlWaterByMoisture();
};
