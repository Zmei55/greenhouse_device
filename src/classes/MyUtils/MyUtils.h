#pragma once

#include <stdint.h>

#include "../../sensors/ds3231.h"

class MyUtils
{
public:
    MyUtils();

    /** Переводит время из DateTime в целое число
     * @param time экземпляр класса DateTime
     */
    uint32_t getNowTimeToInt(const DateTime &time);

    /** Включение освещения
     * @param flag включено ли освещение (bool)
     */
    void enablingLighting(bool &flag);

    /** Выключение освещения
     * @param flag включено ли освещение (bool)
     */
    void disablingLighting(bool &flag);

    /** Включение полива
     * @param flag включен ли полив (bool)
     */
    void enablingWatering(bool &flag);

    /** Выключение полива
     * @param flag включен ли полив (bool)
     */
    void disablingWatering(bool &flag);

    /** Открывание окна
     * @param flag включен ли мотор (bool)
     */
    void openingWindow(bool &flag);

    /** Закрывание окна
     * @param flag включен ли мотор (bool)
     */
    void closingWindow(bool &flag);
};
