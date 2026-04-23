#pragma once

#include "config.h"
#include "./sensors/ds3231.h"

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
    data["isEnabled"] = *isWorkTimeEnabled;
    data["start"] = nullptr;
    data["end"] = nullptr;

    if (!start->isEmpty()) {
        JsonObject startTime = data["start"].to<JsonObject>();
        startTime["hour"] = start->getHour();
        startTime["minute"] = start->getMinute();
    }

    if (!end->isEmpty()) {
        JsonObject endTime = data["end"].to<JsonObject>();
        endTime["hour"] = end->getHour();
        endTime["minute"] = end->getMinute();
    }

    return data;
}

/** 
 * Получение состояния сенсоров
 * true - включен
 * false - выключен
 */
JsonDocument getSensorsValue() {
    JsonDocument data;
    data["soilMoisture"] = *hasSoilMoistureSensor;
    data["photo"] = *hasPhotoSensor;
    data["temperature"] = *hasTemperatureSensor;
    return data;
}

void apiHandler(){
    /** Проверка авторизации (подключился ли аппарат) */
    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;
        data["isLogged"] = true;
        
        request->send(200, "application/json", data.as<String>());
    });

    /** Отправка на клиент данных с включенных сенсоров и текущего времени устройства */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;
        char buf[] = "YYYY-MM-DDThh:mm:ss";

        data["soilMoisture"] = nullptr;
        data["temperature"] = nullptr;
        data["deviceDateTime"] = rtc.now().toString(buf); // Данные датчика реального времени

        /** Данные датчика влажности почвы */
        if (*hasSoilMoistureSensor) {
            data["soilMoisture"] = analogRead(SOIL_MOISTURE_PIN);
        }

        /** Данные датчика температуры */
        if (*hasTemperatureSensor) {
            term.requestTemp();
            term.waitReady();
            if (term.readTemp()) data["temperature"] = term.getTemp();
        }

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

    /** Отправка на клиент настроек */
    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
        JsonDocument data;

        /** Какие сенсоры включены, а какие выключены */
        data["sensors"] = getSensorsValue();

        /** Получение значения температуры, для управления ч-либо */
        data["controlTemperature"] = *controlTemperature;

        /** Получение значения интервала проверки сенсоров */
        data["controlTime"] = *controlTime;

        /** Рабочее время аппарата */
        data["workingHours"] = getWorkingTimeAsJson();

        request->send(200, "application/json", data.as<String>());
    });

    /**
     * Получение с клиента новых настроек
     * Установка новых настроек
     * @param json объект с новыми настройками
     * @return новый объект настроек
     */
    server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){
        JsonDocument data;
        JsonDocument error;

        JsonObject body = json.as<JsonObject>();

        /** Включение / выключение сенсоров */
        JsonObject sensors = body["sensors"];
        *hasSoilMoistureSensor = sensors["soilMoisture"];
        *hasPhotoSensor = sensors["photo"];
        *hasTemperatureSensor = sensors["temperature"];
        /** Установка значение контрольной температуры */
        *controlTemperature = body["controlTemperature"];
        /** Установка значение контрольного времени (интервала проверки показаний датчиков) */
        *controlTime = body["controlTime"];

        /** Установка рабочего времени */
        JsonObject workingHours = body["workingHours"];
        bool isEnabled = workingHours["isEnabled"];
        
        if (isEnabled) {
            int8_t start_hour = workingHours["start"]["hour"];
            int8_t start_minute = workingHours["start"]["minute"];
            int8_t end_hour = workingHours["end"]["hour"];
            int8_t end_minute = workingHours["end"]["minute"];

            try {
                start->set(start_hour, start_minute);
                end->set(end_hour, end_minute);
                *isWorkTimeEnabled = isEnabled;
            } catch(const std::exception& e) {
                error["message"] = e.what();
            }
        } else {
            *isWorkTimeEnabled = false;
            start->reset();
            end->reset();
        }

        /** Какие сенсоры включены, а какие выключены */
        data["sensors"] = getSensorsValue();

        /** Получение значения температуры, для управления ч-либо */
        data["controlTemperature"] = *controlTemperature;

        /** Получение значения интервала проверки сенсоров */
        data["controlTime"] = *controlTime;

        /** Рабочее время аппарата */
        data["workingHours"] = getWorkingTimeAsJson();

        if (error.isNull()) {
            request->send(200, "application/json", data.as<String>());
        } else {
            request->send(400, "application/json", error.as<String>());
        }
    });



    /** TESTS: Тестирование оборудования */

    /** Тестирование освещения (включение светодиодной ленты) */
    server.on("/tests/led-strips/on", HTTP_GET, [](AsyncWebServerRequest *request){
        utils.enablingLighting(isLedStripsOn);
        request->send(200);
    });

    /** Тестирование освещения (выключение светодиодной ленты) */
    server.on("/tests/led-strips/off", HTTP_GET, [](AsyncWebServerRequest *request){
        utils.disablingLighting(isLedStripsOn);
        request->send(200);
    });

    /** Тестирование водяного насоса (включение насоса) */
    server.on("/tests/water-pump/on", HTTP_GET, [](AsyncWebServerRequest *request){
        utils.enablingWatering(isWaterOn);
        request->send(200);
    });

    /** Тестирование водяного насоса (выключение насоса) */
    server.on("/tests/water-pump/off", HTTP_GET, [](AsyncWebServerRequest *request){
        utils.disablingWatering(isWaterOn);
        request->send(200);
    });

    /**
     * Тестирование мотора (открытие окна)
     * @param json объект с указанием времени, в течении которого мотор открывает окно (в секундах)
     */
    server.on("/tests/window/open", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){
        JsonObject body = json.as<JsonObject>();
        *runningWindowMotorTime = body["runningTime"];
        utils.openingWindow(isWindowOpen);

        request->send(200);
    });

    /**
     * Тестирование мотора (закрытие окна)
     * @param json объект с новыми настройками
     */
    server.on("/tests/window/close", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json){
        JsonObject body = json.as<JsonObject>();
        *runningWindowMotorTime = body["runningTime"];
        utils.closingWindow(isWindowOpen);

        request->send(200);
    });
}
