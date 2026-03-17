#include "dht22.h"

#include "config.h"

#define DHTTYPE DHT22

DHT dht(DHT22_PIN, DHTTYPE);

/** Возвращает значение температуры воздуха */
float getTemperature() {
    return dht.readTemperature();
}

/** Возвращает значение влажности воздуха */
float getHumidity() {
    return dht.readHumidity();
}
