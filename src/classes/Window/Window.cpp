#include "Window.h"

Window::Window() {}

Window::Window(uint8_t motorPinOne, uint8_t motorPinTwo) {
    _motorPinOne = motorPinOne;
    _motorPinTwo = motorPinTwo;
}

uint8_t Window::getMotorPinOne() {
    return _motorPinOne;
}

uint8_t Window::getMotorPinTwo() {
    return _motorPinTwo;
}

void Window::open() {
    if (_isMotorOn) throw std::runtime_error("Мотор уже запущен.");
    if (_isWindowOpen) throw std::runtime_error("Окно уже открыто.");

    analogWrite(_motorPinOne, 250);
    digitalWrite(_motorPinTwo, LOW);
    toggleMotorState();
    setMotorStartTime();
}

void Window::close() {
    if (_isMotorOn) throw std::runtime_error("Мотор уже запущен.");
    if (!_isWindowOpen) throw std::runtime_error("Окно уже закрыто.");
    
    digitalWrite(_motorPinOne, LOW);
    analogWrite(_motorPinTwo, 250);
    toggleMotorState();
    setMotorStartTime();
}

void Window::stopMotor() {
    analogWrite(_motorPinOne, 0);
    analogWrite(_motorPinTwo, 0);
    setMotorOff();
}

uint32_t Window::getRunningMotorTime() {
    return _runningWindowMotorTime;
}

uint32_t Window::setRunningMotorTime(uint32_t runningTime) {
    _runningWindowMotorTime = runningTime;
    return _runningWindowMotorTime;
}

bool Window::getIsMotorOn() {
    return _isMotorOn;
}

void Window::toggleMotorState() {
    _isMotorOn = !_isMotorOn;
}

void Window::setMotorOff() {
    _isMotorOn = false;
}

bool Window::getIsWindowOpen() {
    return _isWindowOpen;
}

void Window::toggleWindowState() {
    _isWindowOpen = !_isWindowOpen;
}

uint32_t Window::getMotorStartTime() {
    return _motorStartTime;
}

void Window::setMotorStartTime() {
    _motorStartTime = millis();
}

void Window::resetMotorStartTime() {
    _motorStartTime = 0;
}
