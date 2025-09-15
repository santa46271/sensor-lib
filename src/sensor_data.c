#include "sensor_data.h"

#define MAX_ENCODABLE_TEMP +35
#define MIN_ENCODABLE_TEMP -10
#define TEMP_TABLE_OFFSET (MIN_ENCODABLE_TEMP<0 ? -MIN_ENCODABLE_TEMP : MIN_ENCODABLE_TEMP)
#define BATT_TABLE_OFFSET 5

static const char temperature_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopq";
static const char battery_table[] = " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmno pqrstuvwxyz{|}~";


void init_measurements(Measurements* meas) {
    if (!meas) return;
    meas->cur_index = 0;
}

void init_sensor_data(Sensor_data* data) {
    if (!data) return;
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

char encode_temperature(int8_t t) {
    t = t < MIN_ENCODABLE_TEMP ? MIN_ENCODABLE_TEMP : t;
    t = t > MAX_ENCODABLE_TEMP ? MAX_ENCODABLE_TEMP : t;
    return temperature_table[t+TEMP_TABLE_OFFSET]; 
}

char encode_battery(uint8_t b) {
    if(b>100) return 0xFF;
    if(b<BATT_TABLE_OFFSET) return battery_table[0];
    return battery_table[b-BATT_TABLE_OFFSET];
}

int8_t decode_temperature(char ch) {
    char* pos = strchr(temperature_table, ch);
    if(!pos) {
        return ERR_UNKNOWN_CODE;
    }
    
    return (pos-temperature_table) - TEMP_TABLE_OFFSET;
}

uint8_t decode_battery(char ch) {
    char* pos = strchr(battery_table, ch);
    if(!pos) {
        return ERR_UNKNOWN_CODE;
    }
    
    return (pos-battery_table) + BATT_TABLE_OFFSET;
}

void write_to_buffer(const Sensor_data* header, char* buff, size_t* ind) {
    if(!header || !buff) return;
    buff[(*ind)++] = encode_temperature(header->room1);
    buff[(*ind)++] = encode_temperature(header->room2);
    buff[(*ind)++] = encode_temperature(header->room3);
    buff[(*ind)++] = encode_battery(header->battery);
    buff[*ind] = '\0';
}

void pack(const Measurements* meas, char* buff, size_t buff_size) {
    if(!meas || !buff) return;
    size_t cur_ind =0;
    int size = meas->cur_index<buff_size ? meas->cur_index : buff_size;
    for(int i=0; i<size; i++) {
        write_to_buffer(&meas->sensor_datas[i], buff, &cur_ind);
    }
}

void unpack(const char* buff, Measurements* output) {
    if(!buff || !output) return;
    clear(output);
    
    int size = strlen(buff);
    if(size%4 != 0) return;
    for(int i=0; i<size; ) {
        Sensor_data temp;
        temp.room1 = decode_temperature(buff[i++]);
        temp.room2 = decode_temperature(buff[i++]);
        temp.room3 = decode_temperature(buff[i++]);
        temp.battery = decode_battery(buff[i++]);
        measurements_add_sensor_data(output, &temp);
    }
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
