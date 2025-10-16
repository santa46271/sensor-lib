#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sensor_data.h"

#define BUFF_SIZE 140

int8_t gsm7_pack(const Measurements* meas, char* buff);
int8_t gsm7_unpack(const char* buff, Measurements* meas);

#ifdef __cplusplus
}
#endif
