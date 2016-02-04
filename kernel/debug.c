#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "debug.h"

#define _IN(symbol, p) ((uint32_t)(p)>=(uint32_t)((symbol)->start) && (uint32_t)(p)<=(uint32_t)((symbol)->end))

uint32_t symbols_count = 0;
symbol_t* symbols = 0;

void dbg_init(elf_t* elf) {
  if (!elf) {
    printf("dbg: no debug symbols found\n");
    return;
  }

  printf("dbg: loading debug symbols\n");

  // Load section header entries
  elf32_shdr_t* entries = elf->addr;
  uint32_t entry_size = elf->size;
  uint32_t entries_count = elf->num;

  // Load section header strings entry
  elf32_shdr_t* strings_entry = elf_get_entry(entry_size, entries, elf->shndx);

  // Load symtab entry
  elf32_shdr_t* symtab_entry = 0;
  for (uint32_t i = 0; i < entries_count; i++) {
    elf32_shdr_t* entry = elf_get_entry(entry_size, entries, i);
    char* name = elf_get_string(strings_entry, entry->sh_name);

    if (!strcmp(name, ".symtab")) {
      symtab_entry = entry;
    }
  }

  // Load symtab strings entry
  elf32_shdr_t* symtab_strings_entry = elf_get_entry(entry_size, entries,
      symtab_entry->sh_link);

  // Load symtab entries
  elf32_sym_t* symtab_entries = (elf32_sym_t*) symtab_entry->sh_addr;
  uint32_t symtab_entry_size = symtab_entry->sh_entsize;
  uint32_t symtab_entries_count = symtab_entry->sh_size / symtab_entry_size;

  printf("dbg: %u symbols found\n", symtab_entries_count);

  // Count global functions
  uint32_t global_func_count = 0;
  for (uint32_t i = 0; i < symtab_entries_count; i++) {
    elf32_sym_t* sym_entry = elf_get_sym_entry(symtab_entry_size,
        symtab_entries, i);

    if (elf_get_sym_entry_bind(sym_entry) == STB_GLOBAL
        && elf_get_sym_entry_type(sym_entry) == STT_FUNC) {

      global_func_count++;
    }
  }
  printf("dbg: %u global functions found\n", global_func_count);

  // Load global functions in stack
  symbol_t global_funcs[global_func_count];
  global_func_count = 0;
  for (uint32_t i = 0; i < symtab_entries_count; i++) {
    elf32_sym_t* sym_entry = elf_get_sym_entry(symtab_entry_size,
        symtab_entries, i);

    if (elf_get_sym_entry_bind(sym_entry) == STB_GLOBAL
        && elf_get_sym_entry_type(sym_entry) == STT_FUNC) {

      global_funcs[global_func_count].start = (void*) sym_entry->st_value;
      global_funcs[global_func_count].end = (void*) (sym_entry->st_value
          + sym_entry->st_size - 1);
      strncpy(global_funcs[global_func_count].name,
          elf_get_string(symtab_strings_entry, sym_entry->st_name),
          sizeof(global_funcs[global_func_count].name));

      global_func_count++;
    }
  }

  // Copy global functions to heap
  symbols_count = global_func_count + 1;
  symbols = (symbol_t*) malloc(symbols_count * sizeof(symbol_t));
  symbols[0].start = 0;
  symbols[0].end = 0;
  strcpy(symbols[0].name, "???");
  memcpy(symbols + 1, global_funcs, sizeof(global_funcs));
}

symbol_t* dbg_symbol_at(void* address) {
  for (uint32_t i = 1; i < symbols_count; i++) {
    symbol_t* symbol = symbols + i;

    if (_IN(symbol, address)) {
      return symbol;
    }
  }

  return symbols + 0;
}
