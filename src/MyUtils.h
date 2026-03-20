#pragma once

#include <stdint.h>

class MyUtils
{
public:
    MyUtils();

    /** Управляет освещением растений */
    void lightingPlant();

    /** Управление открыванием окна в зависимости от температуры */
    void controlMotorByTemperature(bool *flag);

    /** Включение полива при сухой почве */
    void enablingWaterByMoisture(bool *flag);

    /** Выключение полива при сухой почве */
    void disablingWaterByMoisture(bool *flag);
};
