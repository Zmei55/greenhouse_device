#pragma once

#include <stdint.h>

/** Предустановки времени, используемые в приложении */
enum TimeApp: uint32_t {
    ONE_SECOND = 1000,
    ONE_MINUTE = ONE_SECOND * 60,
    FIFTEEN_MINUTE = ONE_MINUTE * 15,
    HALF_HOUR = ONE_MINUTE * 30,
    ONE_HOUR = ONE_MINUTE * 60,
};

/** Уровни влажности почвы */
enum SoilMoistureLevel: uint16_t {
    DRY = 3710,
    WET = 1490,
};
