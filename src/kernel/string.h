#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdint.h>

#define string_to_hex(dev, buffer, val, padding)  DEV_CN(dev, to_hex, buffer, val, padding)
#define string_length(dev, str)                   DEV_CN(dev, length, str)
#define string_reverse(dev, dest, src)            DEV_CN(dev, reverse, dest, src)
#define string_copy(dev, dest, src)               DEV_CN(dev, copy, dest, src)

typedef struct _string {
  DEV_FN(void, to_hex, char buffer[9], uint32_t val, uint8_t padding);
  DEV_FN(uint32_t, length, const char* str);
  DEV_FN(void, reverse, char* dest, const char* src);
  DEV_FN(void, copy, char* dest, const char* src);
} string;

#endif
