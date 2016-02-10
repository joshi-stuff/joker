#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "twa.h"
#include "debug.h"

#define _IN(symbol, p) ((uint32_t)(p)>=(uint32_t)((symbol)->start) && (uint32_t)(p)<=(uint32_t)((symbol)->end))

uint32_t symbols_count = 0;
symbol_t* symbols = 0;

void dbg_init_1(elf_t* elf) {
  if (!elf) {
    printf("dbg: no debug symbols found\n");
    return;
  }

  printf("dbg: loading debug symbols to twa\n");

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
  printf("dbg: %u symbols available\n", symtab_entries_count);

  // Count significant symbols
  symbols_count = 0;
  for (uint32_t i = 0; i < symtab_entries_count; i++) {
    elf32_sym_t* sym_entry = elf_get_sym_entry(symtab_entry_size,
        symtab_entries, i);

    if (elf_get_sym_entry_bind(sym_entry) == STB_GLOBAL
        && elf_get_sym_entry_type(sym_entry) == STT_FUNC) {

      symbols_count++;
    }
  }
  printf("dbg: %u significant symbols loaded\n", symbols_count);

  // Load significant symbols in twa
  symbols = twa_alloc(symbols_count * sizeof(symbol_t));
  symbols_count = 0;
  for (uint32_t i = 0; i < symtab_entries_count; i++) {
    elf32_sym_t* sym_entry = elf_get_sym_entry(symtab_entry_size,
        symtab_entries, i);

    if (elf_get_sym_entry_bind(sym_entry) == STB_GLOBAL
        && elf_get_sym_entry_type(sym_entry) == STT_FUNC) {

      symbols[symbols_count].start = (void*) sym_entry->st_value;
      symbols[symbols_count].end = (void*) (sym_entry->st_value
          + sym_entry->st_size - 1);
      strncpy(symbols[symbols_count].name,
          elf_get_string(symtab_strings_entry, sym_entry->st_name),
          sizeof(symbols[symbols_count].name));

      symbols_count++;
    }
  }
}

void dbg_init_2() {
  if (!symbols) {
    return;
  }

  printf("dbg: moving symbols from twa to heap\n");

  // Alloc memory for symbols in heap
  symbol_t* twa_symbols = symbols;
  symbols_count++;
  symbols = (symbol_t*) malloc(symbols_count * sizeof(symbol_t));

  // Insert symbol 0 (unknown)
  symbols[0].start = 0;
  symbols[0].end = 0;
  strcpy(symbols[0].name, "???");

  // Move rest of symbols from twa to heap
  memcpy(symbols + 1, twa_symbols, (symbols_count - 1) * sizeof(symbol_t));
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

void dbg_dump(void* address, size_t size) {
  uint8_t* p = (uint8_t*) address;

  printf("[%X:%X] ", p, p + size - 1);
  for (size_t i = 0; i < size; i++) {
    printf("%02X", p[i]);
  }
  printf("\n");
}

