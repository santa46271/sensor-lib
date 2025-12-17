#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#define ERR_UNKNOWN_CODE 110

static const char hex_table[] = "0123456789ABCDEF";

uint8_t hex_convert(char byte, char* output, size_t size);
size_t hex_convert_str(const char* str, size_t str_size, char* out, size_t out_size);
uint8_t hex_uncode(char ch);
size_t hex_convert_to_byte(const char* hex, size_t hex_size, void* out_bytes);

#ifdef __cplusplus 
}
#endif
