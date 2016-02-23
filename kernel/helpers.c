#include <stdint.h>

#include "helpers.h"

size_t _get_call_stack(size_t max_depth, void** return_addresses) {
  register stack_frame_t* fp asm("ebp");
  stack_frame_t* frame = fp;
  size_t depth;

  for (depth = 0; (depth < max_depth) && frame; depth++) {
    return_addresses[depth] = frame->ret;
    frame = frame->next;
  }

  return depth;
}

void *_memset(void *b, int c, size_t len) {
  uint8_t* p = (uint8_t*) b;

  while ((len--) > 0) {
    *(p++) = c;
  }

  return b;
}

void *_memcpy(void *restrict dst, const void *restrict src, size_t n) {
  uint8_t* ps = (uint8_t*) src;
  uint8_t* pd = (uint8_t*) dst;

  while ((n--) > 0) {
    *(pd++) = *(ps++);
  }

  return dst;
}

size_t _strlen(const char *s) {
  size_t len = 0;
  while (s[len++])
    ;
  return len - 1;
}

char *_strcpy(char * dst, const char * src) {
  char* ret = dst;
  while (*src) {
    *(dst++) = *(src++);
  }
  return ret;
}

char *_strncpy(char * dst, const char * src, size_t len) {
  char* ret = dst;
  while (*src && (len > 1)) {
    *(dst++) = *(src++);
    len--;
  }
  *dst = 0;
  return ret;
}

int _strcmp(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (*s1 != *s2)
      break;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

char *_strrev(char *str) {
  char *p1, *p2;

  if (!str || !*str)
    return str;

  for (p1 = str, p2 = str + _strlen(str) - 1; p2 > p1; ++p1, --p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
  }

  return str;
}

void _format_unsigned(uint64_t number, bool base16, int width, char buffer[32]) {
  int base = base16 ? 16 : 10;
  char* digits = "0123456789abcdef";

  int i = 0;
  for (; number != 0; number /= base) {
    buffer[i++] = digits[number % base];
  }
  for (; i < width; i++) {
    buffer[i] = '0';
  }
  buffer[i] = 0;

  _strrev(buffer);
}

void _format_signed(int64_t number, bool base16, int width, char buffer[32]) {
  if (number < 0) {
    _format_unsigned(-number, base16, width, buffer);
    size_t len = _strlen(buffer);
    buffer[len + 1] = 0;
    for (int i = len; i > 0; i--) {
      buffer[i] = buffer[i - 1];
    }
    buffer[0] = '-';
  } else {
    _format_unsigned(number, base16, width, buffer);
  }
}

