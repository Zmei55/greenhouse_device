#include "apiHelpers.h"
#include "./enums.h"

JsonDocument getCurrentTimeAsJson() {
    JsonDocument data;
    char buf[] = "YYYY-MM-DDThh:mm:ss";
    data["deviceDateTime"] = rtc.now().toString(buf);
    return data;
}

JsonDocument getSensorsValue() {
    JsonDocument data;
    data["soilMoisture"] = hasSoilMoistureSensorRef;
    data["photo"] = hasPhotoSensorRef;
    data["temperature"] = hasTemperatureSensorRef;
    return data;
}

void saveSensorsValue(const JsonObject &body) {
    hasSoilMoistureSensorRef = body["soilMoisture"];
    hasPhotoSensorRef = body["photo"];
    hasTemperatureSensorRef = body["temperature"];
}

JsonDocument getWateringValuesAsJson() {
    JsonDocument data;
    data["waterPressure"] = watering.getWaterPressureValue();

    JsonObject soil = data["soil"].to<JsonObject>();
    soil["dry"] = watering.getSoilMoistureValue(SoilMoisture::DRY);
    soil["wet"] = watering.getSoilMoistureValue(SoilMoisture::WET);
    return data;
}

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

JsonDocument getSettingsValueAsJson() {
    JsonDocument data;
    data["sensors"] = getSensorsValue(); // Какие сенсоры включены, а какие выключены
    data["controlTemperature"] = controlTemperatureRef; // Получение значения температуры, для управления ч-либо
    data["controlTime"] = controlTimeRef / 1000; // Получение значения интервала проверки сенсоров в секундах
    data["runningTime"] = window.getRunningMotorTime() / 1000; // Получения значения времени, в течении которого работает мотор окна (в секундах)
    data["watering"] = getWateringValuesAsJson();
    data["workingHours"] = getWorkingTimeAsJson(); // Рабочее время аппарата
    return data;
}

uint32_t getRuntimeFromJson(const JsonObject &body) {
    uint32_t runningTime = body["runningTime"];
    if (runningTime <= 0) throw std::runtime_error("API. Время открывания/закрывания окна не может быть равно или меньше нуля.");

    runningTime *= 1000; // переводит секунды в миллисекунды
    return runningTime;
}

void saveWatering(const JsonObject &body, JsonDocument &error) {
    int16_t waterPressure = body["waterPressure"];

    JsonObject soil = body["soil"];
    int16_t drySoil = soil["dry"];
    int16_t wetSoil = soil["wet"];

    try {
        watering.setWaterPressureValue(waterPressure);
        watering.setSoilMoistureValue(SoilMoisture::DRY, drySoil);
        watering.setSoilMoistureValue(SoilMoisture::WET, wetSoil);
    } catch (const std::exception &e) {
        error["message"] = e.what();
    }
}

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

void getIsLightingOnAsJson(JsonDocument &data) {
    JsonObject lightingForTest = data.to<JsonObject>();
    lightingForTest["isOn"] = lighting.getIsLedStripsOn();
}

void getIsLightingOnAsJson(JsonDocument &data, std::string name) {
    JsonObject lightingForTest = data[name].to<JsonObject>();
    lightingForTest["isOn"] = lighting.getIsLedStripsOn();
}

void getIsWaterPumpOnAsJson(JsonDocument &data) {
    JsonObject waterPumpForTest = data.to<JsonObject>();
    waterPumpForTest["isOn"] = watering.getIsPumpOn();
}

void getIsWaterPumpOnAsJson(JsonDocument &data, std::string name) {
    JsonObject waterPumpForTest = data[name].to<JsonObject>();
    waterPumpForTest["isOn"] = watering.getIsPumpOn();
}

void getIsWindowOpenAsJson(JsonDocument &data, std::string name) {
    JsonObject windowForTest = data[name].to<JsonObject>();
    windowForTest["isOpen"] = window.getIsWindowOpen();
}
