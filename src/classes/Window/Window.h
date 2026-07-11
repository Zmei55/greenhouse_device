/**
 * Класс, отвечающий за работу окна
 * Если температура выше установленной (контрольной), то окно открывается,
 * если ниже - закрывается.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

class Window {
  public:
    Window();
    Window(uint8_t motorPinOne, uint8_t motorPinTwo);

    /** Получить значение пина мотора 1 */
    uint8_t getMotorPinOne();

    /** Получить значение пина мотора 2 */
    uint8_t getMotorPinTwo();

    /** Открыть окно */
    void open();

    /** Закрыть окно */
    void close();

    /** Остановить работу мотора */
    void stopMotor();

    /**
     * Получить значение времени работы мотора,
     * за которое он открывает/закрывает окно (в секундах)
     */
    uint32_t getRunningMotorTime();

    /**
     * Установить значение времени работы мотора,
     * за которое он открывает/закрывает окно (в секундах)
     * @param runningTime времени работы мотора (в секундах)
     */
    uint32_t setRunningMotorTime(uint32_t runningTime);

    /** Получить значение состояния мотора (включен/выключен) */
    bool getIsMotorOn();

    /** Отключить мотор */
    void setMotorOff();

    /** Получить значение состояния окна (закрыто/открыто) */
    bool getIsWindowOpen();

    /** Изменить состояние окна (закрыто/открыто) */
    void toggleWindowState();

    /** Получить значение времени начала работы мотора */
    uint32_t getMotorStartTime();

    /** Установить значение времени начала работы мотора */
    void setMotorStartTime();

    /** Сбросить значение времени начала работы мотора */
    void resetMotorStartTime();

  private:
    int8_t _motorPinOne = -1;
    int8_t _motorPinTwo = -1;
    uint32_t _runningWindowMotorTime = 5000; // Время, в течение которого мотор открывает/закрывает окно(в миллисекундах)
    uint32_t _motorStartTime = 0; // Время начала работы мотора (в миллисекундах)
    bool _isMotorOn = false; // Включен ли мотор (по ум: false / выключен)
    bool _isWindowOpen = false; // Открыто ли окно (по ум: false / закрыто)
};
