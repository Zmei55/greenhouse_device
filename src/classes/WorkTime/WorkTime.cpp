#include "WorkTime.h"

WorkTime::WorkTime() {}

WorkTime::WorkTime(int8_t hour, int8_t minute) {
    if (hour < 0 || minute < 0) throw std::runtime_error("Время не может быть отрицательным.");
    _hour = hour;
    _minute = minute;
}

void WorkTime::set(int8_t hour, int8_t minute) {
    if (hour < 0 || minute < 0) throw std::runtime_error("Время не может быть отрицательным.");
    _hour = hour;
    _minute = minute;
}

uint32_t WorkTime::getWorkTimeAsInt() {
    return this->getHour() * 3600 + this->getMinute() * 60;
}

void WorkTime::reset() {
    _hour = -1;
    _minute = -1;
}

uint8_t WorkTime::getHour() {
    return this->_hour;
}

uint8_t WorkTime::getMinute() {
    return this->_minute;
}

bool WorkTime::isEmpty() {
    return (_hour == -1 && _minute == -1) ? true : false;
}
