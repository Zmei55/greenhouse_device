#pragma once

#include "config.h"
#include "ds3231.h"

/**
 * Получает даты и времени устройства (датчика реального времени DS3231) в формате json
 * @return дату и время устройства (JsonDocument)
 */
JsonDocument getCurrentTimeAsJson() {
    JsonDocument data;
    char buf[] = "YYYY-MM-DDThh:mm:ss";
    data["deviceDateTime"] = rtc.now().toString(buf);
    return data;
}

/**
 * Получает рабочее время устройства в формате json
 * @return рабочее время устройства (JsonDocument)
 */
JsonDocument getWorkingTimeAsJson() {
    JsonDocument data;
    data["start"] = nullptr;
    data["end"] = nullptr;

    JsonObject startTime = data["start"].to<JsonObject>();
    startTime["hour"] = start->getHour();
    startTime["minute"] = start->getMinute();

    JsonObject endTime = data["end"].to<JsonObject>();        
    endTime["hour"] = end->getHour();
    endTime["minute"] = end->getMinute();

    return data;
}

void apiHandler(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;

        data["sensors"] = nullptr;
        data["soilMoisture"] = nullptr;
        data["temperature"] = nullptr;
        data["deviceDateTime"] = nullptr;
        data["workingHours"] = nullptr;

        /** Какие сенсоры включены, а какие выключены */
        JsonObject sensors = data["sensors"].to<JsonObject>();
        sensors["soilMoisture"] = hasSoilMoistureSensor;
        sensors["photo"] = hasPhotoSensor;
        sensors["time"] = hasTimeSensor;
        sensors["temperature"] = hasTemperatureSensor;

        /** Данные датчика влажности почвы */
        if (hasSoilMoistureSensor) {
            data["soilMoisture"] = analogRead(SOIL_MOISTURE_PIN);
        }

        /** Данные датчика температуры */
        if (hasTemperatureSensor) {
            term.requestTemp();
            term.waitReady();
            if (term.readTemp()) data["temperature"] = term.getTemp();
        }

        /** Данные датчика реального времени */
        if (hasTimeSensor) {
            char buf[] = "YYYY-MM-DDThh:mm:ss";
            data["deviceDateTime"] = rtc.now().toString(buf);
        }

        /** Рабочее время аппарата */
        data["workingHours"] = getWorkingTimeAsJson();

        request->send(200, "application/json", data.as<String>());
    });

    /** Отправка на клиент текущего времени устройства (датчика реального времени DS3231) */
    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", getCurrentTimeAsJson().as<String>());
    });

    /**
     * Получение с клиента новой даты и времени
     * Установка новой даты и времени
     * @param json массив с датой (2026, 1, 1, 12, 0, 0) -> 01.01.2026 12:00:00
     * @return новую дату и время
     */
    server.on("/time", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){
        JsonArray body = json.as<JsonArray>();
        uint16_t newYear = body[0];
        uint8_t newMonth = body[1];
        uint8_t newDay = body[2];
        uint8_t newHour = body[3];
        uint8_t newMinute = body[4];
        uint8_t newSecond = body[5];
        rtc.adjust(DateTime(newYear, newMonth, newDay, newHour, newMinute, newSecond));

        request->send(200, "application/json", getCurrentTimeAsJson().as<String>());
    });

    /** Отправка на клиент рабочего времени устройства */
    server.on("/working-time", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", getWorkingTimeAsJson().as<String>());
    });

    /**
     * Получение с клиента новой даты и времени
     * Установка новой даты и времени
     * @param json объект с временем начала и окончания рабочего времени
     * @return новый объект
     */
    server.on("/working-time", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){        
        JsonObject body = json.as<JsonObject>();
        uint8_t start_hour = body["start"]["hour"];
        uint8_t start_minute = body["start"]["minute"];
        uint8_t end_hour = body["end"]["hour"];
        uint8_t end_minute = body["end"]["minute"];

        start->update(start_hour, start_minute);
        end->update(end_hour, end_minute);

        request->send(200, "application/json", getWorkingTimeAsJson().as<String>());
    });
}