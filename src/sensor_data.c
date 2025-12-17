#include "sensor_data.h"

void measurements_init(Measurements* meas) {
    if (!meas) return;
    meas->cur_index = 0;
}

void sensor_data_init(Sensor_data* data) {
    if (!data) return;
    data->dt = 0;
    data->room1 = 0;
    data->room2 = 0;
    data->room3 = 0;
    data->battery = 0;
}

void measurements_add_sensor_data(Measurements* meas, const Sensor_data* data) {
    if (!meas || !data) return;
    if(meas->cur_index >= SIZE_BUF_MEASUREMENTS) {
        return;
    }
    meas->sensor_datas[meas->cur_index] = *data;
    meas->cur_index++;
}

void clear(Measurements* meas) {
    if(!meas) return;
    meas->cur_index = 0;
}

// uint32_t sd_to_uint32(const Sensor_data* data) {
//     if (!data) return 0;
//     unsigned res = 0;
//     res |= ((uint32_t)(data->battery) & 0xFF)<< 24;
//     res |= ((uint32_t)(data->room3) & 0xFF)<< 16;
//     res |= ((uint32_t)(data->room2) & 0xFF)<< 8;
//     res |= ((uint32_t)(data->room1) & 0xFF);
//     return res;
// }
//
// void uint32_to_sd(uint32_t ui, Sensor_data* res) {
//     if (res == NULL) return;
//     res->room1 = (int8_t)(ui & 0xFF);
//     res->room2 = (int8_t)((ui >> 8) & 0xFF);
//     res->room3 = (int8_t)((ui >> 16) & 0xFF);
//     res->battery = (int8_t)((ui >> 24) & 0xFF);
// }
