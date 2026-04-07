#pragma once

#include "config.h"
#include "ds3231.h"

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
        JsonObject workingHours = data["workingHours"].to<JsonObject>();

        JsonObject startTime = workingHours["start"].to<JsonObject>();
        startTime["hour"] = start->getHour();
        startTime["minute"] = start->getMinute();

        JsonObject endTime = workingHours["end"].to<JsonObject>();        
        endTime["hour"] = end->getHour();
        endTime["minute"] = end->getMinute();

        request->send(200, "application/json", data.as<String>());
    });

    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;

        char buf[] = "YYYY-MM-DDThh:mm:ss";
        data["deviceDateTime"] = rtc.now().toString(buf);

        request->send(200, "application/json", data.as<String>());
    });

    server.on("/time", HTTP_POST, [](AsyncWebServerRequest *request){
        JsonDocument data;

        data["deviceDateTime"] = "Not implemented yet";

        request->send(200, "application/json", data.as<String>());
    });

    server.on("/working-time", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;
        data["workingHours"] = nullptr;

        /** Новое рабочее время аппарата */
        JsonObject newWorkingHours = data["workingHours"].to<JsonObject>();

        JsonObject startTime = newWorkingHours["start"].to<JsonObject>();
        startTime["hour"] = start->getHour();
        startTime["minute"] = start->getMinute();

        JsonObject endTime = newWorkingHours["end"].to<JsonObject>();        
        endTime["hour"] = end->getHour();
        endTime["minute"] = end->getMinute();

        request->send(200, "application/json", data.as<String>());
    });

    server.on("/working-time", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){
        JsonDocument data;
        data["workingHours"] = nullptr;
        
        JsonObject body = json.as<JsonObject>();
        uint8_t start_hour = body["start"]["hour"];
        uint8_t start_minute = body["start"]["minute"];
        uint8_t end_hour = body["end"]["hour"];
        uint8_t end_minute = body["end"]["minute"];

        start->update(start_hour, start_minute);
        end->update(end_hour, end_minute);

        /** Новое рабочее время аппарата */
        JsonObject newWorkingHours = data["workingHours"].to<JsonObject>();

        JsonObject startTime = newWorkingHours["start"].to<JsonObject>();
        startTime["hour"] = start->getHour();
        startTime["minute"] = start->getMinute();

        JsonObject endTime = newWorkingHours["end"].to<JsonObject>();        
        endTime["hour"] = end->getHour();
        endTime["minute"] = end->getMinute();

        request->send(200, "application/json", data.as<String>());
    });
}