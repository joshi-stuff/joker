#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

typedef struct _symbol_t {
  void* start;
  void* end;
  char name[64];
} symbol_t;

void dbg_init(elf_t* elf);

symbol_t* dbg_symbol_at(void* address);
