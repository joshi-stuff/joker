#include <kernel.h>
#include <mmu.h>

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "_helpers.h"

void abort(void) {
  k_panic("abort called");
}

void *malloc(size_t size) {
  return mmu_alloc(size);
}

void *realloc(void *ptr, size_t size) {
  if (ptr) {
    return mmu_realloc(ptr, size);
  } else {
    return mmu_alloc(size);
  }
}

void free(void *ptr) {
  if (ptr) {
    mmu_free(ptr);
  }
}

int atoi(const char *str)NOT_IMPLEMENTED(atoi)
