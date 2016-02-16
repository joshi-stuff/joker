#include <stdint.h>
#include <stdio.h>

#include "cpu.h"

void cpu_init_1() {
  // TODO: Load a valid IDT
}

void cpu_dump_registers() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  printf("ES=%04X  FS=%04X   GS=%04X  DS=%04X  SS=%04X   CS=%04X\n",
      cpu_state.es, cpu_state.fs, cpu_state.gs, cpu_state.ds, cpu_state.ss,
      cpu_state.cs);
  printf("ESP=%08X  EBP=%08X  ESI=%08X  EDI=%08X\n", cpu_state.esp,
      cpu_state.ebp, cpu_state.esi, cpu_state.edi);
  printf("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\n", cpu_state.eax,
      cpu_state.ebx, cpu_state.ecx, cpu_state.edx);
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
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  idt_entry_t* idt_entries = cpu_state.idt.base;
  size_t idt_entries_count = ((size_t) cpu_state.idt.limit + 1)
      / sizeof(idt_entry_t);

  printf("IDT [%p:%p] (%u entries)\n", cpu_state.idt.base,
      ((uint32_t) cpu_state.idt.base) + cpu_state.idt.limit, idt_entries_count);

  for (size_t i = 0; i < idt_entries_count; i++) {
    idt_entry_t* idt_entry = idt_entries + i;

    const char* type = "???";
    switch (cpu_idt_entry_type(idt_entry)) {
    case IDT_TASK_GATE_32:
      type = "TASK_32";
      break;
    case IDT_INT_GATE_16:
      type = "INT_16";
      break;
    case IDT_TRAP_GATE_16:
      type = "TRAP_16";
      break;
    case IDT_INT_GATE_32:
      type = "INT_32";
      break;
    case IDT_TRAP_GATE_32:
      type = "TRAP_32";
      break;
    }

    printf("%3u [%p|%p] %s PRIV%u %s\n", i, cpu_idt_entry_selector(idt_entry),
        cpu_idt_entry_offset(idt_entry),
        cpu_idt_entry_present(idt_entry) == IDT_PRESENT ? "PRES" : "MISS",
        cpu_idt_entry_privilege(idt_entry) / IDT_PRIV_1, type);
  }
}
