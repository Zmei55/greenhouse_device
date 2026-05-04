/**
 * Класс, отвечает за работу полива.
 * Если почва сухая (измеряется датчиком влажности), то включает насос,
 * если влажная, то выключает насос.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../../enums.h"

class Watering {
  public:
    /**
     * Конструктор класса
     * @param moisturePin пин, по которому принимаются данные от датчика влажности
     * @param pumpOnePin пин, по которому происходит управление насосом
     * @param pumpTwoPin пин, по которому происходит управление насосом
     * @return новый объект полива
     */
    Watering(uint8_t moisturePin, uint8_t pumpOnePin, uint8_t pumpTwoPin);

    /** Получить значение пина датчика влажности */
    uint8_t getMoisturePin();

    /** Включение полива */
    void enable();

    /** Выключение полива */
    void disable();

    /** Получить значение состояния насоса (включен/выключен) */
    bool getPumpState();

    /** Изменить состояние насоса (включен/выключен) */
    void togglePumpState();

    /**
     * Получить значение влажности почвы
     * WET - сухая (например: нужно включить полив)
     * DRY - мокрая (например: нужно выключить полив)
     * @param SoilMoisture enum DRY/WET
     * @return значение влажности
     */
    uint16_t getSoilMoistureValue(SoilMoisture value);

    /**
     * Установить значение, при котором нужно включить/выключить насос
     * WET - сухая (например: нужно включить полив)
     * DRY - мокрая (например: нужно выключить полив)
     * @param value значение влажности, при котором нужно включить/выключить насос
     * @return новое значение влажности, сохранённое в системе
     */
    uint16_t setSoilMoistureValue(SoilMoisture value);

  private:
    int8_t _moisturePin = -1;
    int8_t _pumpOnePin = -1;
    int8_t _pumpTwoPin = -1;
    uint16_t _soilDryValue = 3700; // Значение датчика влажности почвы, при котором выключается полив
    uint16_t _soilWetValue = 2700; // Значение датчика влажности почвы, при котором включается полив
    bool _isPumpOn = false; // Состояние насоса (вкл/выкл); по умолчанию: выкл
};
