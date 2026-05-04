#include "Watering.h"

Watering::Watering(uint8_t moisturePin, uint8_t pumpOnePin, uint8_t pumpTwoPin) {
    _moisturePin = moisturePin;
    _pumpOnePin = pumpOnePin;
    _pumpTwoPin = pumpTwoPin;
}

uint8_t Watering::getMoisturePin() { return _moisturePin; }

void Watering::enable() {
    if (_isPumpOn) throw std::runtime_error("Насос уже запущен.");

    analogWrite(_pumpOnePin, 150);
    digitalWrite(_pumpTwoPin, LOW);
    togglePumpState();
}

void Watering::disable() {
    if (_isPumpOn) throw std::runtime_error("Насос уже остановлен.");

    analogWrite(_pumpOnePin, 0);
    digitalWrite(_pumpTwoPin, LOW);
    togglePumpState();
}

bool Watering::getPumpState() { return _isPumpOn; }

void Watering::togglePumpState() { _isPumpOn = !_isPumpOn; }

uint16_t Watering::getSoilMoistureValue(SoilMoisture value) {
    if (value == SoilMoisture::DRY)
        return _soilDryValue;
    else
        return _soilWetValue;
}

uint16_t Watering::setSoilMoistureValue(SoilMoisture value) {
    if (value == SoilMoisture::DRY)
        return _soilDryValue = value;
    else
        return _soilWetValue = value;
}
