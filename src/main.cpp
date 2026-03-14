#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "lightSensor.h"
#include "dht22.h"
#include "myUtils.h"

MyUtils utils;

const uint32_t ONE_SECOND = 1000;
bool redLightOn = false;
bool whiteLightOn = false;

void setup() {
    Serial.begin(115200);
    pinMode(WHITE_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    if (hasDHT22Sensor) dht.begin();
}

void loop() {
    utils.interval(ONE_SECOND, [](){
        if (hasPhotoSensor) {
            if ((getLightSensorValue() == true) && (whiteLightOn == false)) {
                // Serial.println("It's dark");
                digitalWrite(WHITE_LED_PIN, HIGH);
                whiteLightOn = true;
            }
            if ((getLightSensorValue() == false) && (whiteLightOn == true)) {
                // Serial.println("It's light");
                digitalWrite(WHITE_LED_PIN, LOW);
                whiteLightOn = false;
            }
        }

        if (hasDHT22Sensor) {
            // Serial.println(getTemperature());
            if ((getTemperature() > controlTemperature) && (redLightOn == false)) {
                digitalWrite(RED_LED_PIN, HIGH);
                redLightOn = true;
            }
            if ((getTemperature() < controlTemperature) && (redLightOn == true)) {
                digitalWrite(RED_LED_PIN, LOW);
                redLightOn = false;
            }
        }
    });
}
