#pragma once

#include <stdint.h>

#include "../../sensors/ds3231.h"

class MyUtils {
  public:
    MyUtils();

    /**
     * Переводит время из DateTime в целое число
     * @param time экземпляр класса DateTime
     * @return целое число от 00:00:00
     */
    uint32_t getNowTimeToInt(const DateTime &time);
};
