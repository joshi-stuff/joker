#include <kernel.h>

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "_helpers.h"

void abort(void) {
  k_panic("abort called");
}

void *malloc(size_t size) {
  return k_alloc(size);
}

void *realloc(void *ptr, size_t size) {
  if (ptr) {
    return k_realloc(ptr, size);
  } else {
    return k_alloc(size);
  }
}

void free(void *ptr) {
  if (ptr) {
    k_free(ptr);
  }
}

int atoi(const char *str)NOT_IMPLEMENTED(atoi)
