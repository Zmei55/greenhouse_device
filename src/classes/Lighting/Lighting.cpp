#include "Lighting.h"

Lighting::Lighting() {}

Lighting::Lighting(uint8_t photoSensorPin, uint8_t ledStripOnePin, uint8_t ledStripTwoPin,
                   uint8_t ledStripThreePin) {
    _photoSensorPin = photoSensorPin;
    _ledStripOnePin = ledStripOnePin;
    _ledStripTwoPin = ledStripTwoPin;
    _ledStripThreePin = ledStripThreePin;
}

uint8_t Lighting::getPhotoSensorPin() { return _photoSensorPin; }

uint8_t Lighting::getLedStripOnePin() { return _ledStripOnePin; }

uint8_t Lighting::getLedStripTwoPin() { return _ledStripTwoPin; }

uint8_t Lighting::getLedStripThreePin() { return _ledStripThreePin; }

void Lighting::on() {
    if (_isLedStripsOn) throw std::runtime_error("Освещение уже включено.");

    digitalWrite(_ledStripOnePin, HIGH);
    digitalWrite(_ledStripTwoPin, HIGH);
    digitalWrite(_ledStripThreePin, HIGH);
    toggleLedStripsState();
}

void Lighting::off() {
    if (!_isLedStripsOn) throw std::runtime_error("Освещение уже выключено.");

    digitalWrite(_ledStripOnePin, LOW);
    digitalWrite(_ledStripTwoPin, LOW);
    digitalWrite(_ledStripThreePin, LOW);
    toggleLedStripsState();
}

bool Lighting::getLedStripsState() { return _isLedStripsOn; }

void Lighting::toggleLedStripsState() { _isLedStripsOn = !_isLedStripsOn; }
