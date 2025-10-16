#include "gsm7.h"
#include "gsm7_table.h"
#include "hex_format.h"
#include <stdint.h>

#define MAX_ENCODABLE_TEMP +35
#define MIN_ENCODABLE_TEMP -10
#define TEMP_TABLE_OFFSET (MIN_ENCODABLE_TEMP<0 ? -MIN_ENCODABLE_TEMP : MIN_ENCODABLE_TEMP)
#define BATT_TABLE_OFFSET 0

uint8_t get_index(uint8_t code) {
    code &= 0x7F;
    for (uint8_t i =0; i < GSM7_TABLE_SIZE; i++) {
        if(gsm7_table[i] == code) {
            return i;
        }
    }

    return 0xFF;
};

uint8_t get_code(uint8_t index) {
    if(index >= GSM7_TABLE_SIZE) return 0xFF;
    return gsm7_table[index];
}

uint8_t encode_temperature(int8_t t) {
    t = t < MIN_ENCODABLE_TEMP ? MIN_ENCODABLE_TEMP : t;
    t = t > MAX_ENCODABLE_TEMP ? MAX_ENCODABLE_TEMP : t;
    return get_code(t+TEMP_TABLE_OFFSET); 
}

uint8_t encode_battery(uint8_t b) {
    if(b>100) return 0xFF;
    if(b<BATT_TABLE_OFFSET) return get_code(0);
    return get_code(b-BATT_TABLE_OFFSET);
}

int8_t decode_temperature(uint8_t code) {
    uint8_t pos = get_index(code);
    if(pos == 0xFF) {
        return ERR_UNKNOWN_CODE;
    }
    
    return pos - TEMP_TABLE_OFFSET;
}

uint8_t decode_battery(uint8_t code) {
    uint8_t pos = get_index(code);
    if(pos == 0xFF) {
        return ERR_UNKNOWN_CODE;
    }
    
    return pos + BATT_TABLE_OFFSET;
}

void pack_to_hex(const Sensor_data* sd, char* buff, uint8_t* offset) {
    uint8_t pos = *offset;

    uint8_t code = encode_temperature(sd->room1); 
    hex_convert(code, buff+pos, 2);
    pos+=2;

    code = encode_temperature(sd->room2);
    hex_convert(code, buff+pos, 2);
    pos+=2;

    code = encode_temperature(sd->room3);
    hex_convert(code, buff+pos, 2);
    pos+=2;

    code = encode_battery(sd->battery);
    hex_convert(code, buff+pos, 2);
    pos+=2;

    // buff[pos] = '\0';

    *offset = pos;
}

int8_t gsm7_pack(const Measurements* meas, char* buff) {
    if(!meas || !buff) return 1;
    uint8_t cur_ind =0;
    int size = meas->cur_index;
    for(uint8_t i=0; i<size; i++) {
        pack_to_hex(&meas->sensor_datas[i], buff, &cur_ind);
    }

    buff[cur_ind] = '\0';

    return 0;
}

int8_t gsm7_unpack(const char* buff, Measurements* output) {
    if(!buff || !output) return 1;
    clear(output);
    
    uint16_t size = strlen(buff);
    if(size%8 != 0) return 1;
    for(int i=0; i<size; ) {
        Sensor_data temp;

        uint8_t code = hex_uncode(buff[i++])<<4 | hex_uncode(buff[i++]);
        if(code == ERR_UNKNOWN_CODE) return 1;
        temp.room1 = decode_temperature(code);

        code = hex_uncode(buff[i++])<<4 | hex_uncode(buff[i++]);
        if(code == ERR_UNKNOWN_CODE) return 1;
        temp.room2 = decode_temperature(code);

        code = hex_uncode(buff[i++])<<4 | hex_uncode(buff[i++]);
        if(code == ERR_UNKNOWN_CODE) return 1;
        temp.room3 = decode_temperature(code);

        code = hex_uncode(buff[i++])<<4 | hex_uncode(buff[i++]);
        if(code == ERR_UNKNOWN_CODE) return 1;
        temp.battery = decode_battery(code);

        measurements_add_sensor_data(output, &temp);
    }

    return 0;
}
