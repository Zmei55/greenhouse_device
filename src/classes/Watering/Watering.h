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
     * @return новый объект полива
     */
    Watering();
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

    /** Получить значение пина насоса 1 */
    uint8_t getPumpOnePin();

    /** Получить значение пина насоса 2 */
    uint8_t getPumpTwoPin();

    /** Включение полива */
    void enable();

    /** Выключение полива */
    void disable();

    /** Получить значение состояния насоса (true - включен / false - выключен) */
    bool getIsPumpOn();

    /** Изменить состояние насоса (включен/выключен) */
    void togglePumpState();

    /** Получить значение сухая ли почва true/false */
    bool isSoilWet();

    /** Получить значение мокрая ли почва true/false */
    bool isSoilDry();

    /**
     * Получить значение влажности почвы установленные в устройстве
     * WET - сухая (например: нужно включить полив)
     * DRY - мокрая (например: нужно выключить полив)
     * @param SoilMoisture enum DRY/WET
     * @return значение влажности
     */
    uint16_t getSoilMoistureValue(SoilMoisture moisture);

    /**
     * Установить значение, при котором нужно включить/выключить насос
     * WET - сухая (например: нужно включить полив)
     * DRY - мокрая (например: нужно выключить полив)
     * @param value значение влажности, при котором нужно включить/выключить насос
     * @return новое значение влажности, сохранённое в системе
     */
    uint16_t setSoilMoistureValue(SoilMoisture moisture, uint16_t value);

    /**
     * Получить значение давления воды
     * @return значение давления воды
     */
    uint16_t getWaterPressureValue();

    /**
     * Установить значение давления воды
     * @param newValue значение давления воды
     * @return новое значение давления воды, сохранённое в системе
     */
    uint16_t setWaterPressureValue(int16_t newValue);

  private:
    int8_t _moisturePin = -1;
    int8_t _pumpOnePin = -1;
    int8_t _pumpTwoPin = -1;
    int16_t _waterPressure = 100;
    uint16_t _soilDryValue = 3700; // Значение датчика влажности почвы, при котором выключается полив
    uint16_t _soilWetValue = 2700; // Значение датчика влажности почвы, при котором включается полив
    bool _isPumpOn = false; // Состояние насоса (вкл/выкл); по умолчанию: выкл
};
