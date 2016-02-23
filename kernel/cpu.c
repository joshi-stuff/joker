#include <stdint.h>

#include "cpu.h"
#include "kernel.h"
#include "helpers.h"

static idt_t idt;
static idt_entry_t idt_entries[256];

/*
 * 0 - Division by zero exception
 * 1 - Debug exception
 * 2 - Non maskable interrupt
 * 3 - Breakpoint exception
 * 4 - 'Into detected overflow'
 * 5 - Out of bounds exception
 * 6 - Invalid opcode exception
 * 7 - No coprocessor exception
 * 8 - Double fault (pushes an error code)
 * 9 - Coprocessor segment overrun
 * 10 - Bad TSS (pushes an error code)
 * 11 - Segment not present (pushes an error code)
 * 12 - Stack fault (pushes an error code)
 * 13 - General protection fault (pushes an error code)
 * 14 - Page fault (pushes an error code)
 * 15 - Unknown interrupt exception
 * 16 - Coprocessor fault
 * 17 - Alignment check exception
 * 18 - Machine check exception
 * 19-31 - Reserved
 */
#define DEFINE_INT_HANDLER(n)\
  void default_int_##n##_handler() {\
    k_ensure_abort("handler for interrupt " #n " not set", __FILE__, __LINE__);\
  }
DEFINE_INT_HANDLER(0)
DEFINE_INT_HANDLER(1)
DEFINE_INT_HANDLER(2)
DEFINE_INT_HANDLER(3)
DEFINE_INT_HANDLER(4)
DEFINE_INT_HANDLER(5)
DEFINE_INT_HANDLER(6)
DEFINE_INT_HANDLER(7)
DEFINE_INT_HANDLER(8)
DEFINE_INT_HANDLER(9)
DEFINE_INT_HANDLER(10)
DEFINE_INT_HANDLER(11)
DEFINE_INT_HANDLER(12)
DEFINE_INT_HANDLER(13)
DEFINE_INT_HANDLER(14)
DEFINE_INT_HANDLER(15)
DEFINE_INT_HANDLER(16)
DEFINE_INT_HANDLER(17)
DEFINE_INT_HANDLER(18)
DEFINE_INT_HANDLER(19)
DEFINE_INT_HANDLER(20)
DEFINE_INT_HANDLER(21)
DEFINE_INT_HANDLER(22)
DEFINE_INT_HANDLER(23)
DEFINE_INT_HANDLER(24)
DEFINE_INT_HANDLER(25)
DEFINE_INT_HANDLER(26)
DEFINE_INT_HANDLER(27)
DEFINE_INT_HANDLER(28)
DEFINE_INT_HANDLER(29)
DEFINE_INT_HANDLER(30)
DEFINE_INT_HANDLER(31)
DEFINE_INT_HANDLER(32)

#define SET_INT_HANDLER(n) cpu_set_idt_gate(n, cpu_state.cs, default_int_##n##_handler, IDT_PRIV_0, IDT_INT_GATE_32);

void cpu_init_1() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  _memset(idt_entries, 0, sizeof(idt_entries));

  SET_INT_HANDLER(0);
  SET_INT_HANDLER(1);
  SET_INT_HANDLER(2);
  SET_INT_HANDLER(3);
  SET_INT_HANDLER(4);
  SET_INT_HANDLER(5);
  SET_INT_HANDLER(6);
  SET_INT_HANDLER(7);
  SET_INT_HANDLER(8);
  SET_INT_HANDLER(9);
  SET_INT_HANDLER(10);
  SET_INT_HANDLER(11);
  SET_INT_HANDLER(12);
  SET_INT_HANDLER(13);
  SET_INT_HANDLER(14);
  SET_INT_HANDLER(15);
  SET_INT_HANDLER(16);
  SET_INT_HANDLER(17);
  SET_INT_HANDLER(18);
  SET_INT_HANDLER(19);
  SET_INT_HANDLER(20);
  SET_INT_HANDLER(21);
  SET_INT_HANDLER(22);
  SET_INT_HANDLER(23);
  SET_INT_HANDLER(24);
  SET_INT_HANDLER(25);
  SET_INT_HANDLER(26);
  SET_INT_HANDLER(27);
  SET_INT_HANDLER(28);
  SET_INT_HANDLER(29);
  SET_INT_HANDLER(30);
  SET_INT_HANDLER(31);
  SET_INT_HANDLER(32);

  idt.base = idt_entries;
  idt.limit = sizeof(idt_entries) - 1;

  asm volatile("lidt [%0]" :: "r" (&idt));
}

void cpu_set_idt_gate(uint8_t num, uint16_t selector, void* ptr,
    uint8_t privilege, uint8_t type) {

  idt_entry_t* e = idt_entries + num;

  e->zero = 0;
  e->selector = selector;
  e->offset_low = ((uint32_t) ptr) & 0xFFFF;
  e->offset_high = ((uint32_t) ptr) >> 16;
  e->flags = IDT_PRESENT | privilege | type;

//  if(num<10) printf("cpu_set_idt_gate: %u %p %X %X\n", num, ptr, e->offset_low, e->offset_high);
}

void cpu_dump_registers() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  k_printf("ES=%04X  FS=%04X   GS=%04X  DS=%04X  SS=%04X   CS=%04X\n",
      cpu_state.es, cpu_state.fs, cpu_state.gs, cpu_state.ds, cpu_state.ss,
      cpu_state.cs);
  k_printf("ESP=%08X  EBP=%08X  ESI=%08X  EDI=%08X\n", cpu_state.esp,
      cpu_state.ebp, cpu_state.esi, cpu_state.edi);
  k_printf("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\n", cpu_state.eax,
      cpu_state.ebx, cpu_state.ecx, cpu_state.edx);
  k_printf("CR0=%08X  CR2=%08X  CR3=%08X  CR4=%08X\n", cpu_state.cr0,
      cpu_state.cr2, cpu_state.cr3, cpu_state.cr4);
  k_printf("EFL=%08X  GDT=%08X  IDT=%08X\n", cpu_state.eflags, cpu_state.gdt,
      cpu_state.idt);
}

void cpu_dump_gdt() {
  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  gdt_entry_t* gdt_entries = cpu_state.gdt.base;
  size_t gdt_entries_count = ((size_t) cpu_state.gdt.limit + 1)
      / sizeof(gdt_entry_t);

  k_printf("GDT [%p:%p] (%u entries)\n", cpu_state.gdt.base,
      ((uint32_t) cpu_state.gdt.base) + cpu_state.gdt.limit, gdt_entries_count);

  for (size_t i = 0; i < gdt_entries_count; i++) {
    gdt_entry_t* gdt_entry = gdt_entries + i;

    k_printf("    [%p:%p] |%04X:| %s %s %s PRIV%u %s %s\n",
        cpu_gdt_entry_base(gdt_entry),
        cpu_gdt_entry_granularity(gdt_entry) == GDT_GRANULARITY_PAGE ?
            (uint8_t*) cpu_gdt_entry_base(gdt_entry)
                + (cpu_gdt_entry_limit(gdt_entry) << 12 | 0xFFF) :
            (uint8_t*) cpu_gdt_entry_base(
                gdt_entry) + cpu_gdt_entry_limit(gdt_entry),
        i * sizeof(gdt_entry_t),
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

  k_printf("IDT [%p:%p] (%u entries)\n", cpu_state.idt.base,
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

    k_printf("%3u |%04X:%p| %s PRIV%u %s\n", i,
        cpu_idt_entry_selector(idt_entry), cpu_idt_entry_offset(idt_entry),
        cpu_idt_entry_present(idt_entry) == IDT_PRESENT ? "PRES" : "MISS",
        cpu_idt_entry_privilege(idt_entry) / IDT_PRIV_1, type);
  }
}

/* http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html */
