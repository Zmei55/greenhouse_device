/** DS3231 - датчик реального времени */
#pragma once

#include <RTClib.h>

extern RTC_DS3231 rtc;

/** Возвращает текущее время аппарата */
char* getCurrentDeviceDateTime();
