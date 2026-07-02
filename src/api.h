#pragma once

#include "./sensors/ds3231.h"
#include "./sensors/thermometer.h"
#include "apiHelpers.h"
#include "config.h"

void apiHandler() {
    /** Проверка авторизации (подключился ли аппарат) */
    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument data;
        data["isLogged"] = true;

        request->send(200, "application/json", data.as<String>());
    });

    /** Отправка на клиент данных с включенных сенсоров и текущего времени устройства */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument data;
        char buf[] = "YYYY-MM-DDThh:mm:ss";

        data["soilMoisture"] = nullptr;
        data["temperature"] = nullptr;
        data["deviceDateTime"] = rtc.now().toString(buf); // Данные датчика реального времени

        /** Данные датчика влажности почвы */
        if (hasSoilMoistureSensorRef) {
            data["soilMoisture"] = analogRead(SOIL_MOISTURE_PIN);
        }

        /** Данные датчика температуры */
        if (hasTemperatureSensorRef) {
            term.requestTemp();
            term.waitReady();
            if (term.readTemp()) data["temperature"] = term.getTemp();
        }

        request->send(200, "application/json", data.as<String>());
    });

    /** Отправка на клиент текущего времени устройства (датчика реального времени DS3231) */
    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", getCurrentTimeAsJson().as<String>());
    });

    /**
     * Установка новой даты и времени,
     * полученной от клиента
     * @param json массив с датой (2026, 1, 1, 12, 0, 0) -> 01.01.2026 12:00:00
     * @return новую дату и время
     */
    server.on("/time", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
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
    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", getSettingsValueAsJson().as<String>());
    });

    /**
     * Получить с клиента новых настроек
     * Установка новых настроек
     * @param json объект с новыми настройками
     * @return новый объект настроек
     */
    server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonDocument error;
        JsonObject body = json.as<JsonObject>();

        controlTemperatureRef = body["controlTemperature"]; // Установка значение контрольной температуры
        controlTimeRef = (int)body["controlTime"] * 1000; // Установка значение контрольного времени (интервала проверки показаний датчиков) в миллисекундах
        uint32_t runningTime = getRuntimeFromJson(body["runningTime"]);
        window.setRunningMotorTime(runningTime); // Установка значение времени работы мотора окна в миллисекундах
        saveSensorsValue(body["sensors"]); // Включение / выключение сенсоров
        saveWatering(body["watering"], error); // Установка значений влажности почвы, при которых нужно включать и выключать полив
        saveWorkingTime(body["workingHours"], error); // Установка рабочего времени

        if (error.isNull()) {
            request->send(200, "application/json", getSettingsValueAsJson().as<String>());
        } else {
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TESTS: Тестирование оборудования */
    /** Отправка первоначального состояния устройства: что включено, а что выключено в данный момент */
    server.on("/tests/all", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            JsonDocument data;
            getIsLightingOnAsJson(data, "lighting");
            getIsWaterPumpOnAsJson(data, "waterPump");
            getIsWindowOpenAsJson(data, "window");
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TEST: Тестирование освещения (включение светодиодной ленты) */
    server.on("/tests/led-strips/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            lighting.on();
            JsonDocument data;
            getIsLightingOnAsJson(data);
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TEST: Тестирование освещения (выключение светодиодной ленты) */
    server.on("/tests/led-strips/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            lighting.off();
            JsonDocument data;
            getIsLightingOnAsJson(data);
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TEST: Тестирование водяного насоса (включение насоса) */
    server.on("/tests/water-pump/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            watering.enable();
            JsonDocument data;
            getIsWaterPumpOnAsJson(data);
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TEST: Тестирование водяного насоса (выключение насоса) */
    server.on("/tests/water-pump/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            watering.disable();
            JsonDocument data;
            getIsWaterPumpOnAsJson(data);
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /**
     * TEST: Тестирование мотора (открытие окна)
     * @param json объект с указанием времени, в течении которого мотор открывает окно (в секундах)
     */
    server.on("/tests/window/open", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
        try {
            uint32_t runningTime = getRuntimeFromJson(json.as<JsonObject>());
            window.setRunningMotorTime(runningTime);
            window.open();

            JsonDocument data;
            JsonObject windowForTest = data.to<JsonObject>();
            windowForTest["isOpen"] = true;
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /**
     * TEST: Тестирование мотора (закрытие окна)
     * @param json объект с новыми настройками
     */
    server.on("/tests/window/close", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
        try {
            uint32_t runningTime = getRuntimeFromJson(json.as<JsonObject>());
            window.setRunningMotorTime(runningTime);
            window.close();

            JsonDocument data;
            JsonObject windowForTest = data.to<JsonObject>();
            windowForTest["isOpen"] = false;
            request->send(200, "application/json", data.as<String>());
        } catch (const std::exception &e) {
            JsonDocument error;
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });
}
