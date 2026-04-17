#pragma once

#include <stdint.h>
#include <stdexcept>

class WorkTime {
    public:
    /** Создание НЕ инициализированный объект */
    WorkTime();

    /**
     * Создание и инициализация объекта
     * @param hour часы;
     * @param minute минуты;
     */
    WorkTime(int8_t hour, int8_t minute);

    /**
     * Установка данных для объекта
     * @param hour часы;
     * @param minute минуты;
     */
    void set(int8_t hour, int8_t minute);

    /** Сброс данных объекта */
    void reset();

    /** Возвращает значение часов */
    uint8_t getHour();

    /** Возвращает значение минут */
    uint8_t getMinute();

    /** Возвращает значение пустой объект или нет */
    bool isEmpty();

    private:
    int8_t _hour = -1;
    int8_t _minute = -1;
};
