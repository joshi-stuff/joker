#include <stdbool.h>
#include <stddef.h>

#include "kernel.h"
#include "string.h"

DEFINE_DRIVER(string);

DEFINE_DEVICE(string, string,
);

static void to_hex(void* dev, char buffer[9], uint32_t val, uint8_t justify);
static uint32_t length(void* dev, const char* str);
static void reverse(void* dev, char* revbuffer, const char* buffer);
static void copy(void* dev, char* dest, const char* src);

static const char* hex_digits = "0123456789ABCDEF";

/* TODO: this should be in the heap */
static string_string default_string_string = {
  {
    to_hex, length, reverse, copy
  }
};


void load() {
  set_device(string, NULL, &default_string_string);
}

void unload() {
  remove_device(string, NULL);
}

void on_set_device(symbol class, symbol id, device* dev) {
}

void on_remove_device(symbol class, symbol id) {
}

void to_hex(void* dev, char buffer[9], uint32_t val, uint8_t padding) {
  char revbuffer[9];
  uint8_t i = 0;

  if (val == 0) {
    buffer[i++] = '0';
  }
  else {
    while (val != 0) {
      uint32_t remainder = val % 16;
      buffer[i++] = hex_digits[remainder];
      val /= 16;
    }
  }

  while (i < padding) {
    buffer[i++] = '0';
  }

  buffer[i] = 0;

  reverse(dev, revbuffer, buffer);
  copy(dev, buffer, revbuffer);
}

uint32_t length(void* dev, const char* str) {
  uint32_t len = 0;
  for (; *str; len++, str++);
  return len;
}

void reverse(void* dev, char* dest, const char* src) {
  int j;
  int i = length(dev, src);

  for (j=0; j<i; j++) {
    dest[j] = src[i-1-j];
  }

  dest[j] = 0;
}

void copy(void* dev, char* dest, const char* src) {
  while (*src) {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = 0;
}
