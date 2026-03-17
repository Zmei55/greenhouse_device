#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "config.h"

/** Возвращает булевое значение освещенности
 * настраивается на датчике, переменным резистором
 * true -> темно
 * false -> светло
 */
bool getLightSensorValue();

#endif
