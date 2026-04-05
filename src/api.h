#pragma once

#include "config.h"
#include "ds3231.h"

void apiHandler(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;
        JsonDocument workingHours;
        JsonDocument startTime;
        JsonDocument endTime;

        struct StartTime {
            uint8_t hour = 9; // Час включения аппарата (09)
            uint8_t minute = 0; // Минуты включения аппарата (00)
        } start;
        struct EndTime {
            uint8_t hour = 20; // Час выключения аппарата (20)
            uint8_t minute = 0; // Минуты включения аппарата (00)
        } end;

        data["soilMoisture"] = nullptr;
        data["temperature"] = nullptr;
        data["deviceDateTime"] = nullptr;
        data["workingHours"] = nullptr;

        if (hasSoilMoistureSensor) {
            data["soilMoisture"] = analogRead(SOIL_MOISTURE_PIN);
        }

        if (hasTemperatureSensor) {
            term.requestTemp();
            term.waitReady();
            if (term.readTemp()) data["temperature"] = term.getTemp();
        }

        if (hasTimeSensor) {
            char buf[] = "YYYY-MM-DDThh:mm:ss";
            data["deviceDateTime"] = rtc.now().toString(buf);
        }

        startTime["hour"] = start.hour;
        startTime["minute"] = start.minute;
        endTime["hour"] = end.hour;
        endTime["minute"] = end.minute;
        workingHours["start"] = startTime;
        workingHours["end"] = endTime;
        data["workingHours"] = workingHours;

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
}