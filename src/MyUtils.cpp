#include "MyUtils.h"

#include <Arduino.h>

#include "config.h"
#include "lightSensor.h"
#include "dht22.h"

MyUtils::MyUtils() {}

void MyUtils::interval(uint32_t delay, void (*callback)()) {
    static uint32_t lastTime = millis();

    if (millis() - lastTime >= delay) {
        lastTime = millis();
        callback();
    }
}

void MyUtils::timer(uint32_t delay, void (*callback)()) {
    static uint32_t lastTime = millis();

    if (millis() - lastTime >= delay) {
        callback();
    }
}

void MyUtils::lightingPlant() {
    static bool ledStripsOn = false;

    /** Включение освещения при темноте */
    if ((getLightSensorValue() == true) && (ledStripsOn == false)) {
        if (ledStripOneEnabled) digitalWrite(LED_STRIP_ONE_PIN, HIGH);
        if (ledStripTwoEnabled) digitalWrite(LED_STRIP_TWO_PIN, HIGH);
        if (ledStripThreeEnabled) digitalWrite(LED_STRIP_THREE_PIN, HIGH);
        ledStripsOn = true;
    }
    /** Выключение освещения при свете */
    if ((getLightSensorValue() == false) && (ledStripsOn == true)) {
        digitalWrite(LED_STRIP_ONE_PIN, LOW);
        digitalWrite(LED_STRIP_TWO_PIN, LOW);
        digitalWrite(LED_STRIP_THREE_PIN, LOW);
        ledStripsOn = false;
    }
}

void MyUtils::controlMotorByTemperature() {
    static bool redLightOn = false;

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

void MyUtils::controlWaterByMoisture() {
    static bool waterOn = false;

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
