#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "lightSensor.h"
#include "ds3231.h"
#include "api.h"

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);

    rtc.begin();
    rtc.adjust(DateTime(2026, 1, 1, 12, 0, 0)); // Установка времени устройства по умолчанию
    
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    
    term.setResolution(12); // Установка разрешения датчика температуры (9-12 бит, чем выше, тем точнее, но дольше измерение)
    term.requestTemp(); // Запрос на измерение температуры (необходимо для получения данных при первом вызове term.tick())

    apiHandler();
    server.begin();
}

void loop() {    
}


