#include <stdio.h>
#include <stdint.h>

#include "kernel.h"
#include "twa.h"

typedef struct _twa_t {
  uint32_t size;
  uint32_t used;
} twa_t;

static twa_t* twa = 0;

void twa_init(void* twa_start, void* twa_end) {
  twa = (twa_t*) twa_start;
  twa->size = (uint32_t) twa_end - (uint32_t) twa_start + 1;
  twa->used = sizeof(twa_t);
  printf("twa: allocating %uKB for twa\n", twa->size / 1024);
}

void twa_destroy() {
  printf("twa: destroyed twa\n");
  twa = 0;
}

void* twa_alloc(size_t size) {
  k_ensure(twa != 0);
  k_ensure((twa->used + size) <= twa->size);

  void* p = ((uint8_t *) twa) + twa->used;
  twa->used += size;

  return p;
}

void twa_free(size_t size) {
  k_ensure(twa != 0);
  twa->used -= size;
}

