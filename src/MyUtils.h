#pragma once

#include <stdint.h>

#include "ds3231.h"

class MyUtils
{
public:
    MyUtils();

    /** Переводит время из DateTime в целое число
     * @param time экземпляр класса DateTime
     */
    uint32_t getNowTimeToInt(const DateTime& time);

    /** Включение освещения
     * @param flag включено ли освещение (bool)
     */
    void enablingLighting(bool *flag);

    /** Выключение освещения
     * @param flag включено ли освещение (bool)
     */
    void disablingLighting(bool *flag);

    /** Включение полива
     * @param flag включен ли полив (bool)
     */
    void enablingWatering(bool *flag);

    /** Выключение полива
     * @param flag включен ли полив (bool)
     */
    void disablingWatering(bool *flag);

    /** Включение мотора
     * @param flag включен ли мотор (bool)
     */
    void enablingMotor(bool *flag);

    /** Выключение мотора
     * @param flag включен ли мотор (bool)
     */
    void disablingMotor(bool *flag);
};
