#include "WorkTime.h"

WorkTime::WorkTime() {}

WorkTime::WorkTime(uint8_t hour, uint8_t minute) {
    _hour = hour;
    _minute = minute;
}

WorkTime WorkTime::set(uint8_t hour, uint8_t minute) {
    return {_hour, _minute};
}

WorkTime WorkTime::update(uint8_t hour, uint8_t minute) {
    return {_hour, _minute};
}

uint8_t WorkTime::getHour() {
    return this->_hour;
}

uint8_t WorkTime::getMinute() {
    return this->_minute;
}
