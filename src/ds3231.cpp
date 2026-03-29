#include "ds3231.h"

RTC_DS3231 rtc;

char* getCurrentDeviceDateTime() {
    char buf[] = "YYYYMMDD-hh:mm:ss";
    return rtc.now().toString(buf);
}
