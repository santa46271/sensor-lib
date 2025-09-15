#pragma once

#include <stdint.h>
#include <string.h>

#define HOURS 24
#define MEASUREMENTS_HOUR 4
#define SIZE_BUF_MEASUREMENTS (MEASUREMENTS_HOUR * HOURS)
#define ERR_UNKNOWN_CODE 110

typedef struct Sensor_data {
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
void pack(const Measurements* meas, char* buff, size_t buff_size);
void unpack(const char* buff, Measurements* output);
