#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

typedef struct _symbol_t {
  void* start;
  void* end;
  char name[64];
} symbol_t;

void dbg_init_1(elf_t* elf);
void dbg_init_2();

symbol_t* dbg_symbol_at(void* address);
void dbg_dump(void* address, size_t size);
