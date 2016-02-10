#include <stdint.h>
#include <stdio.h>

#include "cpu.h"

void cpu_dump_registers() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  printf("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\n", cpu_state.eax,
      cpu_state.ebx, cpu_state.ecx, cpu_state.edx);
  printf("ESP=%08X  EBP=%08X  ESI=%08X  EDI=%08X\n", cpu_state.esp,
      cpu_state.ebp, cpu_state.esi, cpu_state.edi);
  printf("ES =%08X  FS =%08X  GS =%08X\n", cpu_state.es, cpu_state.fs,
      cpu_state.gs);
  printf("DS =%08X  SS =%08X  CS =%08X\n", cpu_state.ds, cpu_state.ss,
      cpu_state.cs);
  printf("CR0=%08X  CR2=%08X  CR3=%08X  CR4=%08X\n", cpu_state.cr0,
      cpu_state.cr2, cpu_state.cr3, cpu_state.cr4);
  printf("EFL=%08X  GDT=%08X  IDT=%08X\n", cpu_state.eflags, cpu_state.gdt,
      cpu_state.idt);
}

void cpu_dump_gdt() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  gdt_entry_t* gdt_entries = cpu_state.gdt.base;
  size_t gdt_entries_count = ((size_t) cpu_state.gdt.limit + 1)
      / sizeof(gdt_entry_t);

  printf("GDT [%p:%p] (%u entries)\n", cpu_state.gdt.base,
      ((uint32_t) cpu_state.gdt.base) + cpu_state.gdt.limit, gdt_entries_count);

  for (size_t i = 0; i < gdt_entries_count; i++) {
    gdt_entry_t* gdt_entry = gdt_entries + i;

    printf("%3u [%p:%p] %s %s %s PRIV%u %s %s\n", i,
        cpu_gdt_entry_base(gdt_entry),
        cpu_gdt_entry_granularity(gdt_entry) == GDT_GRANULARITY_PAGE ?
            (uint8_t*) cpu_gdt_entry_base(gdt_entry)
                + (cpu_gdt_entry_limit(gdt_entry) << 12 | 0xFFF) :
            (uint8_t*) cpu_gdt_entry_base(
                gdt_entry) + cpu_gdt_entry_limit(gdt_entry),
        cpu_gdt_entry_granularity(gdt_entry) == GDT_GRANULARITY_PAGE ?
            "PAGE" : "BYTE",
        cpu_gdt_entry_mode(gdt_entry) == GDT_MODE_32 ? "32bit" : "16bit",
        cpu_gdt_entry_present(gdt_entry) == GDT_PRESENT ? "PRES" : "MISS",
        cpu_gdt_entry_privilege(gdt_entry) / GDT_PRIV_1,
        cpu_gdt_entry_executable(gdt_entry) == GDT_EXECUTABLE ? "EXEC" : "DATA",
        cpu_gdt_entry_executable(gdt_entry) ?
            (cpu_gdt_entry_dc(gdt_entry) == GDT_CONFORMING ? "CNF" : "") :
            (cpu_gdt_entry_dc(gdt_entry) == GDT_DIR_DOWN ? "DOWN" : "UP"),
        cpu_gdt_entry_executable(gdt_entry) ?
            (cpu_gdt_entry_rw(gdt_entry) == GDT_READABLE ? "READ" : "") :
            (cpu_gdt_entry_rw(gdt_entry) == GDT_WRITABLE ? "WRITE" : "R/O"),
        cpu_gdt_entry_dirty(gdt_entry) == GDT_DIRTY ? "DIRTY" : "CLEAN");
  }
}

void cpu_dump_idt() {

}
