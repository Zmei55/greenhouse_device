#include "MyUtils.h"

#include <Arduino.h>

#include "config.h"

MyUtils::MyUtils() {}

uint32_t MyUtils::getNowTimeToInt(const DateTime &time) {
    return time.hour() * 3600 + time.minute() * 60;
}
