#pragma once

#include <stdint.h>

class MyUtils
{
public:
    MyUtils();

    /** Управляет освещением растений */
    void lightingPlant();

    /** Управление открыванием окна в зависимости от температуры */
    void controlMotorByTemperature();

    /** Управление поливом в зависимости от влажности почвы */
    void enablingWaterByMoisture(bool *flag);

    // void wateringPlant();
};
