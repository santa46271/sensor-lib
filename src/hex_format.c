#ifdef __cplusplus
extern "C" {
#endif

#include "hex_format.h"

uint8_t hex_convert(char byte, char* output, size_t size) {
    if(size<2) return 1;
    output[0] = hex_table[byte>>4];
    output[1] = hex_table[byte & 0x0F];
    return 0;
}

size_t hex_convert_str(const char* str, size_t str_size, char* out, size_t out_size) {
    if(str == NULL || out ==NULL) return 0;
    for (size_t i = 0; i<str_size; i++) {
        uint8_t err = hex_convert(str[i], &out[i*2], 2);
        if(err) return i;
    }
    return str_size;
}

uint8_t hex_uncode(char ch) {
    char* pos = strchr(hex_table, ch);
    if(!pos) {
        return ERR_UNKNOWN_CODE;
    }
    
    return pos-hex_table;
}

size_t hex_convert_to_byte(const char* hex, size_t hex_size, void* out_bytes) {
    if(hex_size<2 || hex == NULL || out_bytes == NULL) return 0;
    size_t writed_bytes =0;
    uint8_t* bytes = (uint8_t*)out_bytes;
    for(size_t i =0; i<hex_size; i+=2) {
        uint8_t byte = 0;
        uint8_t uncode = hex_uncode(hex[i]);
        if(uncode == ERR_UNKNOWN_CODE) {
            return writed_bytes;
        }

        byte = uncode<<4;
        uncode = hex_uncode(hex[i+1]);
        if (uncode == ERR_UNKNOWN_CODE) {
            return writed_bytes;
        }
        byte |= uncode & 0x0F;

        bytes[writed_bytes++] = byte;
    }

    return writed_bytes;
}


#ifdef __cplusplus 
}
#endif
