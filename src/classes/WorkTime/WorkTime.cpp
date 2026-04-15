#include "WorkTime.h"

WorkTime::WorkTime() {}

WorkTime::WorkTime(uint8_t hour, uint8_t minute) {
    _hour = hour;
    _minute = minute;
}

WorkTime WorkTime::set(uint8_t hour, uint8_t minute) {
    return WorkTime(_hour, _minute);
}

WorkTime WorkTime::update(uint8_t newHour, uint8_t newMinute) {
    _hour = newHour;
    _minute = newMinute;
    return WorkTime(_hour, _minute);
}

uint8_t WorkTime::getHour() {
    return this->_hour;
}

uint8_t WorkTime::getMinute() {
    return this->_minute;
}

bool WorkTime::isEmpty() {
    return (this->_hour == 0 && this->_minute == 0) ? true : false;
}
