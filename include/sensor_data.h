#pragma once

#ifdef __cplusplus
extern "C"
#endif

#include <stdint.h>
// #include <string.h>
#include "date_time.h"

#define HOURS 24
#define MEASUREMENTS_HOUR 4
#define SIZE_BUF_MEASUREMENTS (MEASUREMENTS_HOUR * HOURS)


typedef struct Sensor_data {
    Date_time dt;
    int8_t room1;
    int8_t room2;
    int8_t room3;
    uint8_t battery;
} Sensor_data;

typedef struct Measurements{
    Sensor_data sensor_datas[SIZE_BUF_MEASUREMENTS];
    int cur_index;
} Measurements;

void init_measurements(Measurements* meas);
void init_sensor_data(Sensor_data* data);
void measurements_add_sensor_data(Measurements* meas, const Sensor_data* data);
void clear(Measurements* meas);

#ifdef __cplusplus
}
#endif
