#pragma once

#include <stdint.h>

class WorkTime {
    public:
    /** Создание НЕ инициализированный объект */
    WorkTime();

    /**
     * Создание и инициализация объекта
     * @param hour часы;
     * @param minute минуты;
     */
    WorkTime(uint8_t hour, uint8_t minute);

    /**
     * Инициализация объекта, ранее созданного
     * @param hour часы;
     * @param minute минуты;
     * @return инициализированный экземпляр класса WorkTime
     */
    WorkTime set(uint8_t hour, uint8_t minute);

    /**
     * Обновление данных ранее созданного объекта
     * @param hour часы;
     * @param minute минуты;
     * @return обновленный экземпляр класса WorkTime
     */
    WorkTime update(uint8_t hour, uint8_t minute);

    /** Возвращает значение часов */
    uint8_t getHour();

    /** Возвращает значение минут */
    uint8_t getMinute();

    private:
    uint8_t _hour;
    uint8_t _minute;
};
