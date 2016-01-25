#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "_helpers.h"

uint8_t* mempool = (uint8_t*) 100000;

void abort(void) {
  printf("Uh, oh, abort called :-(\n");
  while(1);
}

void *malloc(size_t size) {
  void* block = mempool;

  mempool += size;

  DBG("malloc: return %X of %d bytes\n", block, size);

  return block;
}

void *realloc(void *ptr, size_t size)NOT_IMPLEMENTED(realloc)

void free(void *ptr) {
  DBG("free: free %X\n", ptr);
}

int atoi(const char *str)NOT_IMPLEMENTED(atoi)
