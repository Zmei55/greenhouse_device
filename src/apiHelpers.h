/** Функции-помощники для обработки API-запросов */
#pragma once

#include "config.h"

/**
 * Получить даты и времени устройства (датчика реального времени DS3231) в формате json
 * @return дату и время устройства (JsonDocument)
 */
JsonDocument getCurrentTimeAsJson();

/**
 * Получить состояние сенсоров
 * true - включен
 * false - выключен
 * @return значения состояния сенсоров (JsonDocument)
 */
JsonDocument getSensorsValue();

/**
 * Сохранить состояние сенсоров
 * @param body значения состояния сенсоров, полученные от клиента (JsonObject)
 * true - включен
 * false - выключен
 */
void saveSensorsValue(const JsonObject &body);

/**
 * Получить значения влажности почвы, при которых включается и выключается полив
 * Получить значения полива
 * WET - сухой
 * DRY - мокрый
 * @return значения влажности почвы (JsonDocument)
 */
JsonDocument getWateringValuesAsJson();

/**
 * Получить состояние настроек
 * @return значения настроек (JsonDocument)
 */
JsonDocument getSettingsValueAsJson();

/**
 * Получить время работы открывания окна из json
 * @param body json-объект, содержащий время, в течении которого надо открывать окно (в секундах)
 * @example getRuntimeFromJson(json.as<JsonObject>());
 * @return время, в течении которого надо открывать окно (в миллисекундах)
 */
uint32_t getRuntimeFromJson(const JsonObject &body);

/**
 * Сохранить значения влажности почвы, при которых включается и выключается полив
 * @param body значения влажности почвы, при которых включается и выключается полив
 * @param error объект для сохранения ошибки, если данные рабочего времени некорректные
 * @throws std::runtime_error если данные влажности некорректные
 */
void saveWatering(const JsonObject &body, JsonDocument &error);

/**
 * Сохранить рабочее время устройства
 * @param body json-объект с данными рабочего времени
 * @param error объект для сохранения ошибки, если данные рабочего времени некорректные
 * @throws std::runtime_error если данные рабочего времени некорректные
 */
void saveWorkingTime(const JsonObject &body, JsonDocument &error);

/**
 * Получить состояние освещения (вкл/выкл)
 * @param data json-объект, в котором надо сохранить данные
 * @param name (не обязат) название поля, в котором сохранять данные
 */
void getIsLightingOnAsJson(JsonDocument &data);
void getIsLightingOnAsJson(JsonDocument &data, std::string name);

/**
 * Получить состояние водяного насоса (вкл/выкл)
 * @param data json-json-объект, в котором надо сохранить данные
 * @param name (не обязат) название поля, в котором сохранять данные
 */
void getIsWaterPumpOnAsJson(JsonDocument &data);
void getIsWaterPumpOnAsJson(JsonDocument &data, std::string name);

/**
 * Получить состояние окна (открыто/закрыто)
 * @param data json-объект, в котором надо сохранить данные
 * @param name название поля, в котором сохранять данные
 */
void getIsWindowOpenAsJson(JsonDocument &data, std::string name);
