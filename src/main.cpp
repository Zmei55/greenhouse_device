#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "dht22.h"
#include "myUtils.h"

MyUtils utils;

bool redLightOn = false;
bool waterOn = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_STRIP_ONE_PIN, OUTPUT);
    pinMode(LED_STRIP_TWO_PIN, OUTPUT);
    pinMode(LED_STRIP_THREE_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    if (hasDHT22Sensor) dht.begin();
}

void loop() {
    utils.interval(controlTime, [](){
        if (hasPhotoSensor) {
            utils.lightingPlant();
        }

        if (hasDHT22Sensor) {
            /** Включение ... если температура выше установленного значения */
            if ((getTemperature() > controlTemperature) && (redLightOn == false)) {
                digitalWrite(RED_LED_PIN, HIGH);
                redLightOn = true;
            }
            /** Выключение ... если температура ниже установленного значения */
            if ((getTemperature() < controlTemperature) && (redLightOn == true)) {
                digitalWrite(RED_LED_PIN, LOW);
                redLightOn = false;
            }
        }

        if (hasSoilMoistureSensor) {
            // Serial.print("1: ");
            // Serial.println(analogRead(SOIL_MOISTURE_PIN));
            // /** Включение полива при сухой почве */
            // if ((analogRead(SOIL_MOISTURE_PIN) > SoilMoistureLevel::DRY) && (waterOn == false)) {
            //     digitalWrite(WATER_PIN, HIGH);
            //     waterOn = true;
            // }

            // if (waterOn == true) {
            //     utils.interval(TimeApp::ONE_SECOND, [](){
            //         Serial.print("2: ");
            //         Serial.println(analogRead(SOIL_MOISTURE_PIN));

            //         /** Выключение полива при влажной почве */
            //         if (analogRead(SOIL_MOISTURE_PIN) < 2000) {
            //             digitalWrite(WATER_PIN, LOW);
            //             waterOn = false;
            //         }
            //     });
            // }

            // utils.interval(TimeApp::ONE_SECOND, [](){
            //     Serial.print("2: ");
            //     Serial.println(analogRead(SOIL_MOISTURE_PIN));
            //     /** Выключение полива при влажной почве */
            //     if ((analogRead(SOIL_MOISTURE_PIN) < SoilMoistureLevel::WET * 2) && (waterOn == true)) {
            //         digitalWrite(WATER_PIN, LOW);
            //         waterOn = false;
            //     }
            // });
        }
    });

    // if (waterOn == true) {
    //     utils.interval(TimeApp::ONE_SECOND, [](){
    //         Serial.print("2: ");
    //         Serial.println(analogRead(SOIL_MOISTURE_PIN));

    //         /** Выключение полива при влажной почве */
    //         if (analogRead(SOIL_MOISTURE_PIN) < SoilMoistureLevel::WET * 2) {
    //             digitalWrite(WATER_PIN, LOW);
    //             waterOn = false;
    //         }
    //     });
    // }
}
