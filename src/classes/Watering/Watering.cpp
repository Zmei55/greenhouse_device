#include "Watering.h"

Watering::Watering() {};

Watering::Watering(uint8_t moisturePin, uint8_t pumpOnePin, uint8_t pumpTwoPin) {
    _moisturePin = moisturePin;
    _pumpOnePin = pumpOnePin;
    _pumpTwoPin = pumpTwoPin;
}

uint8_t Watering::getMoisturePin() { return _moisturePin; }

uint8_t Watering::getPumpOnePin() { return _pumpOnePin; }

uint8_t Watering::getPumpTwoPin() { return _pumpTwoPin; }

void Watering::enable() {
    if (_isPumpOn) throw std::runtime_error("Насос уже запущен.");

    analogWrite(_pumpOnePin, 50);
    digitalWrite(_pumpTwoPin, LOW);
    togglePumpState();
}

void Watering::disable() {
    if (!_isPumpOn) throw std::runtime_error("Насос уже остановлен.");

    analogWrite(_pumpOnePin, 0);
    digitalWrite(_pumpTwoPin, LOW);
    togglePumpState();
}

bool Watering::isPumpOn() { return _isPumpOn; }

void Watering::togglePumpState() { _isPumpOn = !_isPumpOn; }

bool Watering::isSoilWet() {
    return analogRead(getMoisturePin()) < _soilWetValue ? true : false;
}

bool Watering::isSoilDry() {
    return analogRead(getMoisturePin()) > _soilDryValue ? true : false;
}

uint16_t Watering::getSoilMoistureValue(SoilMoisture moisture) {
    if (moisture == SoilMoisture::DRY)
        return _soilDryValue;
    else
        return _soilWetValue;
}

uint16_t Watering::setSoilMoistureValue(SoilMoisture moisture, uint16_t value) {
    if (value < 0) throw std::runtime_error("Уровень влажности не может быть меньше нуля.");

    if (moisture == SoilMoisture::DRY) {
        if (value > SoilMoistureLevel::MAX) throw std::runtime_error("Значение влажной почвы не может быть выше максимального.");
        return _soilDryValue = value;
    } else {
        if (value < SoilMoistureLevel::MIN) throw std::runtime_error("Значение сухой почвы не может быть ниже минимального.");
        return _soilWetValue = value;
    }
}

uint16_t Watering::getWaterPressureValue() { return _waterPressure; };

uint16_t Watering::setWaterPressureValue(int16_t newValue) {
    if (newValue < 0) throw std::runtime_error("Давление воды не может быть отрицательным.");

    return _waterPressure = newValue;
};
