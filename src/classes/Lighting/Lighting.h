/**
 * Класс, отвечающий за работу освещения
 * Если естественное освещение ниже установленного, то искусственное освещение
 * включается, если выше - выключается.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

class Lighting {
  public:
    Lighting();
    Lighting(uint8_t photoSensorPin, uint8_t ledStripOnePin, uint8_t ledStripTwoPin,
             uint8_t ledStripThreePin);

    /** Получить значение пина фотосенсора */
    uint8_t getPhotoSensorPin();

    /** Получить значение пина led-полосы 1 */
    uint8_t getLedStripOnePin();

    /** Получить значение пина led-полосы 2 */
    uint8_t getLedStripTwoPin();

    /** Получить значение пина led-полосы 3 */
    uint8_t getLedStripThreePin();

    /** Включить освещение */
    void on();

    /** Выключить освещение */
    void off();

    /** Получить состояние освещения (включено/выключено) */
    bool getLedStripsState();

    /** Изменить состояние освещения (включено/выключено) */
    void toggleLedStripsState();

  private:
    int8_t _photoSensorPin = -1;
    int8_t _ledStripOnePin = -1;
    int8_t _ledStripTwoPin = -1;
    int8_t _ledStripThreePin = -1;
    bool _isLedStripsOn = false;
};
