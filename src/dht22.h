/**
 * DHT22 - датчик, измеряющий температуру и влажность воздуха
 * getTemperature() - возвращает значение температуры воздуха (float)
 * getHumidity() - возвращает значение влажность воздуха (float)
 */
#pragma once

#include <DHT.h>

#define DHTTYPE DHT22
#define DHT22_PIN 20

DHT dht(DHT22_PIN, DHTTYPE);
float temperature, humidity;

/** Возвращает значение температуры воздуха */
float getTemperature() {
    temperature = dht.readTemperature();
    return temperature;
}

/** Возвращает значение влажности воздуха */
float getHumidity() {
    humidity = dht.readHumidity();
    return humidity;
}
