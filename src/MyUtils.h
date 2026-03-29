#pragma once

#include <stdint.h>

#include "ds3231.h"

class MyUtils
{
public:
    MyUtils();

    /** Управляет освещением растений */
    void lightingPlant();

    /** Включение полива при сухой почве */
    void enablingWaterByMoisture(bool *flag);

    /** Выключение полива при сухой почве */
    void disablingWaterByMoisture(bool *flag);

    /** Переводит время из DateTime в целое число */
    uint32_t getNowTimeToInt(DateTime time);

    /** Управление открыванием окна в зависимости от температуры */
    void controlMotorByTemperature(bool *flag);
};
