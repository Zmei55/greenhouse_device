#pragma once

/**
 * Варианты влажности почвы
 * DRY - влажная
 * WET - сухая
 */
enum SoilMoisture {
    DRY,
    WET
};

/** Уровни влажности почвы */
enum SoilMoistureLevel : uint16_t {
    MAX = 3710,
    MIN = 1490,
};
