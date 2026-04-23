/**
 * DS3231 RTC module - датчик реального времени
 * реализация для получения текущего времени с помощью библиотеки RTClib
 */
#pragma once

#include <RTClib.h>

extern RTC_DS3231 rtc;

/** Возвращает текущее время аппарата */
char* getCurrentDeviceDateTime();
