/**
 * DHT22 - датчик, измеряющий температуру и влажность воздуха
 * getTemperature() - возвращает значение температуры воздуха (float)
 * getHumidity() - возвращает значение влажность воздуха (float)
 */
#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>

#include "config.h"

extern DHT dht;

/** Возвращает значение температуры воздуха */
float getTemperature();

/** Возвращает значение влажности воздуха */
float getHumidity();

#endif
