#pragma once

#include "./enums.h"
#include "./sensors/ds3231.h"
#include "./sensors/thermometer.h"
#include "config.h"

JsonDocument getCurrentTimeAsJson();
JsonDocument getSettingsValueAsJson();
void saveSensorsValue(const JsonObject &body);
void saveRuntimeToWindow(const uint8_t &body);
void saveWorkingTime(const JsonObject &body, JsonDocument &error);

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

        saveSensorsValue(body["sensors"]); // Включение / выключение сенсоров
        controlTemperatureRef = body["controlTemperature"]; // Установка значение контрольной температуры
        controlTimeRef = (int)body["controlTime"] * 1000; // Установка значение контрольного времени (интервала проверки показаний датчиков) в миллисекундах
        saveRuntimeToWindow(body["runningTime"]); // Установка значение времени работы мотора окна в миллисекундах

        saveWorkingTime(body["workingHours"], error); // Установка рабочего времени

        if (error.isNull()) {
            request->send(200, "application/json", getSettingsValueAsJson().as<String>());
        } else {
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** TESTS: Тестирование оборудования */

    /** Тестирование освещения (включение светодиодной ленты) */
    server.on("/tests/led-strips/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        lighting.on();
        request->send(200);
    });

    /** Тестирование освещения (выключение светодиодной ленты) */
    server.on("/tests/led-strips/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        lighting.off();
        request->send(200);
    });

    /** Тестирование водяного насоса (включение насоса) */
    server.on("/tests/water-pump/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument error;

        try {
            watering.enable();
            request->send(200);
        } catch (const std::exception &e) {
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /** Тестирование водяного насоса (выключение насоса) */
    server.on("/tests/water-pump/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument error;

        try {
            watering.disable();
            request->send(200);
        } catch (const std::exception &e) {
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /**
     * Тестирование мотора (открытие окна)
     * @param json объект с указанием времени, в течении которого мотор открывает окно (в секундах)
     */
    server.on("/tests/window/open", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonDocument error;
        JsonObject body = json.as<JsonObject>();

        try {
            saveRuntimeToWindow(body["runningTime"]);
            window.open();
            request->send(200);
        } catch (const std::exception &e) {
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });

    /**
     * Тестирование мотора (закрытие окна)
     * @param json объект с новыми настройками
     */
    server.on("/tests/window/close", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonDocument error;
        JsonObject body = json.as<JsonObject>();

        try {
            saveRuntimeToWindow(body["runningTime"]);
            window.close();
            request->send(200);
        } catch (const std::exception &e) {
            error["message"] = e.what();
            request->send(400, "application/json", error.as<String>());
        }
    });
}

/**
 * Получить даты и времени устройства (датчика реального времени DS3231) в формате json
 * @return дату и время устройства (JsonDocument)
 */
JsonDocument getCurrentTimeAsJson() {
    JsonDocument data;
    char buf[] = "YYYY-MM-DDThh:mm:ss";
    data["deviceDateTime"] = rtc.now().toString(buf);
    return data;
}

/**
 * Получить рабочее время устройства в формате json
 * @return рабочее время устройства (JsonDocument)
 */
JsonDocument getWorkingTimeAsJson() {
    JsonDocument data;
    data["isEnabled"] = isWorkTimeEnabledRef;
    data["start"] = nullptr;
    data["end"] = nullptr;

    if (!WTStartRef.isEmpty()) {
        JsonObject startTime = data["start"].to<JsonObject>();
        startTime["hour"] = WTStartRef.getHour();
        startTime["minute"] = WTStartRef.getMinute();
    }

    if (!WTEndRef.isEmpty()) {
        JsonObject endTime = data["end"].to<JsonObject>();
        endTime["hour"] = WTEndRef.getHour();
        endTime["minute"] = WTEndRef.getMinute();
    }

    return data;
}

/**
 * Получить состояние сенсоров
 * true - включен
 * false - выключен
 * @return значения состояния сенсоров (JsonDocument)
 */
JsonDocument getSensorsValue() {
    JsonDocument data;
    data["soilMoisture"] = hasSoilMoistureSensorRef;
    data["photo"] = hasPhotoSensorRef;
    data["temperature"] = hasTemperatureSensorRef;
    return data;
}

/**
 * Сохранить состояние сенсоров
 * @param body значения состояния сенсоров, полученные от клиента (JsonObject)
 * true - включен
 * false - выключен
 */
void saveSensorsValue(const JsonObject &body) {
    hasSoilMoistureSensorRef = body["soilMoisture"];
    hasPhotoSensorRef = body["photo"];
    hasTemperatureSensorRef = body["temperature"];
}

/**
 * Получить значения влажности почвы, при которых включается и выключается полив
 * WET - сухой
 * DRY - мокрый
 * @return значения влажности почвы (JsonDocument)
 */
JsonDocument getSoilMoistureValuesAsJson() {
    JsonDocument data;
    data["dry"] = watering.getSoilMoistureValue(SoilMoisture::DRY);
    data["wet"] = watering.getSoilMoistureValue(SoilMoisture::WET);
    return data;
}

/**
 * Получить состояние настроек
 * @return значения настроек (JsonDocument)
 */
JsonDocument getSettingsValueAsJson() {
    JsonDocument data;
    data["sensors"] = getSensorsValue(); // Какие сенсоры включены, а какие выключены
    data["controlTemperature"] = controlTemperatureRef; // Получение значения температуры, для управления ч-либо
    data["controlTime"] = controlTimeRef / 1000; // Получение значения интервала проверки сенсоров в секундах
    data["runningTime"] = window.getRunningMotorTime() / 1000; // Получения значения времени, в течении которого работает мотор окна (в секундах)
    data["soil"] = getSoilMoistureValuesAsJson();
    data["workingHours"] = getWorkingTimeAsJson(); // Рабочее время аппарата
    return data;
}

/**
 * Получить время работы открывания окна из json
 * @param body время, в течении которого надо открывать окно (в секундах)
 * @return время, в течении которого надо открывать окно (в миллисекундах)
 */
uint32_t getRuntimeFromJson(const uint8_t &body) {
    uint32_t runningTime = body;
    if (runningTime <= 0) throw std::runtime_error("Время открывания/закрывания окна не может быть равно или меньше нуля.");

    runningTime *= 1000; // переводит секунды в миллисекунды
    return runningTime;
}

/**
 * Сохранить время работы открывания окна в объекте Window
 * @param body время, в течении которого надо открывать окно (в секундах)
 */
void saveRuntimeToWindow(const uint8_t &body) {
    uint32_t runningTime = getRuntimeFromJson(body);
    window.setRunningMotorTime(runningTime);
}

/**
 * Сохранить рабочее время устройства
 * @param body объект с данными рабочего времени
 * @param error объект для сохранения ошибки, если данные рабочего времени некорректные
 * @throws std::runtime_error если данные рабочего времени некорректные
 */
void saveWorkingTime(const JsonObject &body, JsonDocument &error) {
    bool isEnabled = body["isEnabled"];

    if (isEnabled) {
        int8_t start_hour = body["start"]["hour"];
        int8_t start_minute = body["start"]["minute"];
        int8_t end_hour = body["end"]["hour"];
        int8_t end_minute = body["end"]["minute"];

        try {
            WTStartRef.set(start_hour, start_minute);
            WTEndRef.set(end_hour, end_minute);
            isWorkTimeEnabledRef = isEnabled;
        } catch (const std::exception &e) {
            error["message"] = e.what();
        }
    } else {
        isWorkTimeEnabledRef = false;
        WTStartRef.reset();
        WTEndRef.reset();
    }
}
