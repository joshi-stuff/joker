#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "cpu.h"
#include "console.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

#define GDT_ENTRIES_COUNT  3
#define GDT_ENTRY_CODE     1
#define GDT_ENTRY_DATA     2
#define IDT_ENTRIES_COUNT  256
#define IDT_USED           0x80
#define IDT_UNUSED         0x00

typedef enum _gdt_presency {
  GDT_PRESENCY_PRESENT = 0x80,
  GDT_PRESENCY_MISSING = 0x00
} gdt_presency;

typedef enum _gdt_priv {
	GDT_PRIV_0 = 0x00,
	GDT_PRIV_1 = 0x20,
	GDT_PRIV_2 = 0x40,
	GDT_PRIV_3 = 0x60
} gdt_priv;

typedef enum _gdt_type {
  GDT_TYPE_CONFORM_EXEC = 0x0C,
  GDT_TYPE_EXEC         = 0x08,
  GDT_TYPE_DATA         = 0x00,
  GDT_TYPE_DATA_STACK   = 0x04
} gdt_type;

typedef enum _gdt_rw {
  GDT_RW_ON  = 0x02,
  GDT_RW_OFF = 0x00
} gdt_rw;

typedef enum _gdt_granularity {
  GDT_GRANULARITY_PAGE = 0x80,
  GDT_GRANULARITY_BYTE = 0x00
} gdt_granularity;

typedef enum _gdt_mode {
  GDT_MODE_32 = 0x40,
  GDT_MODE_16 = 0x00
} gdt_mode;

typedef enum _idt_type {
	IDT_TYPE_TASK_GATE_32 = 0x15,
	IDT_TYPE_INT_GATE_16  = 0x06,
	IDT_TYPE_TRAP_GATE_16 = 0x17,
	IDT_TYPE_INT_GATE_32  = 0x0E,
	IDT_TYPE_TRAP_GATE_32 = 0x1F
} idt_type;

typedef enum _idt_priv {
	IDT_PRIV_0 = 0x00,
	IDT_PRIV_1 = 0x20,
	IDT_PRIV_2 = 0x40,
	IDT_PRIV_3 = 0x60
} idt_priv;

typedef packed_struct _gdt_descriptor {
  uint16_t limit;
  void*    base;
} gdt_descriptor;

typedef packed_struct _gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t  base_middle;
  uint8_t  access;
  uint8_t  granularity;
  uint8_t  base_high;
} gdt_entry;

typedef packed_struct _idt_descriptor {
	uint16_t size;
	void*    address;
} idt_descriptor;

typedef packed_struct _idt_entry {
   uint16_t offset_high;  // offset bits 0..15
   uint16_t selector;     // a code segment selector in GDT or LDT
   uint8_t  zero;         // unused, set to 0
   uint8_t  type_attr;    // type and attributes, see below
   uint16_t offset_low;   // offset bits 16..31
} idt_entry;

DEFINE_DRIVER(x386);

DEFINE_DEVICE(x386, cpu,
  gdt_descriptor gdt_descriptor;
  gdt_entry      gdt_entries[GDT_ENTRIES_COUNT];
  idt_descriptor idt_descriptor;
  idt_entry      idt_entries[IDT_ENTRIES_COUNT];
  void**         idt_handler_bridges;
  void**         idt_handlers;
);

static void set_idt_gate(void* dev, cpu_idt_number n, cpu_idt_handler handler);
static void clear_idt_gate(void* dev, cpu_idt_number n);

static void load_gdt(x386_cpu* x386);
static void set_gdt_entry(x386_cpu* x386, uint32_t n, void* base, uint32_t limit, gdt_presency presency, gdt_priv priv, gdt_type type, gdt_rw rw, gdt_granularity granularity, gdt_mode mode);
static void flush_gdt(x386_cpu* x386);
static void load_idt(x386_cpu* x386);
static void noop_idt_handler();

/* TODO: this should be defined per CPU */
extern void* idt_handler_bridges[];
extern void* idt_handlers[];

/* TODO: this should be in the heap */
static x386_cpu default_x386_cpu = {
  {set_idt_gate, clear_idt_gate},
  {0, NULL},
  {},
  {0, NULL},
  {},
  idt_handler_bridges,
  idt_handlers
};

void load() {
  CLI;
  load_gdt(&default_x386_cpu);
  load_idt(&default_x386_cpu);
  //STI;

  set_device(cpu, NULL, &default_x386_cpu);
}

void unload() {
  remove_device(cpu, NULL);

  for (uint32_t i = 0; i < IDT_ENTRIES_COUNT; i++) {
    clear_idt_gate(&default_x386_cpu, i);
  }
}

void on_set_device(symbol class, symbol id, device* dev) {
}

void on_remove_device(symbol class, symbol id) {
}

void set_idt_gate(void* dev, cpu_idt_number n, cpu_idt_handler handler) {
  DEV_LOAD(x386, cpu);

	CLI;
	cpu->idt_handlers[n] = handler;
	STI;
}

void clear_idt_gate(void* dev, cpu_idt_number n) {
  DEV_LOAD(x386, cpu);

	CLI;
  cpu->idt_handlers[n] = noop_idt_handler;
	STI;
}

void load_gdt(x386_cpu* x386) {
  x386->gdt_descriptor.limit = sizeof(x386->gdt_entries) - 1;
  x386->gdt_descriptor.base = &x386->gdt_entries;

  set_gdt_entry(
      x386, 0, 0x00000000, 0, GDT_PRESENCY_MISSING, GDT_PRIV_0, GDT_TYPE_DATA,
      GDT_RW_OFF, GDT_GRANULARITY_BYTE, GDT_MODE_32);

  set_gdt_entry(
      x386, GDT_ENTRY_CODE, 0x00000000, 0xFFFFFFFF, GDT_PRESENCY_PRESENT,
      GDT_PRIV_0, GDT_TYPE_EXEC, GDT_RW_ON, GDT_GRANULARITY_PAGE, GDT_MODE_32);

  set_gdt_entry(
      x386, GDT_ENTRY_DATA, 0x00000000, 0xFFFFFFFF, GDT_PRESENCY_PRESENT,
      GDT_PRIV_0, GDT_TYPE_DATA, GDT_RW_ON, GDT_GRANULARITY_PAGE, GDT_MODE_32);

  flush_gdt(x386);
}

void set_gdt_entry(x386_cpu* x386, uint32_t n, void* base, uint32_t limit,
    gdt_presency presency, gdt_priv priv, gdt_type type, gdt_rw rw,
    gdt_granularity granularity, gdt_mode mode) {

  gdt_entry* entry = &x386->gdt_entries[n];
  uint32_t _base = (uint32_t)base;

  /* Setup the descriptor base address */
  entry->base_low    = (_base         & 0xFFFF);
  entry->base_middle = ((_base >> 16) & 0xFF);
  entry->base_high   = ((_base >> 24) & 0xFF);

  /* Setup the descriptor limits */
  entry->limit_low   = (limit         & 0xFFFF);
  entry->granularity = ((limit >> 16) & 0x0F);

  /* Setup the access flags */
  entry->access |= (presency    & 0x80);
  entry->access |= (priv        & 0x60);
  entry->access |= (              0x10);
  entry->access |= (type        & 0x0C);
  entry->access |= (rw          & 0x02);

  /* Finally, set up the granularity and type */
  entry->granularity |= (granularity & 0x80);
  entry->granularity |= (mode        & 0x40);
}

void __attribute__ ((noinline)) flush_gdt(x386_cpu* x386) {
  asm volatile (
    "lgdt [%0]\n"
    "mov ds, %1\n"
    "mov es, %1\n"
    "mov fs, %1\n"
    "mov gs, %1\n"
    "mov ss, %1\n"
    "jmp %2:_load_gdt_ret\n"
    "_load_gdt_ret:\n"
    :
    : "r" (&x386->gdt_descriptor),
      "r" (GDT_ENTRY_DATA * sizeof(gdt_entry)),
      "" (GDT_ENTRY_CODE * sizeof(gdt_entry))
  );
}

void load_idt(x386_cpu* x386) {
  x386->idt_descriptor.size = IDT_ENTRIES_COUNT * sizeof(idt_entry);
  x386->idt_descriptor.address = &x386->idt_entries;

  for (uint32_t n = 0; n < IDT_ENTRIES_COUNT; n++) {
    void* idt_handler_bridge = x386->idt_handler_bridges[n];

    x386->idt_entries[n].selector = (GDT_ENTRY_CODE * sizeof(gdt_entry));
    x386->idt_entries[n].zero = 0;
    x386->idt_entries[n].type_attr = IDT_TYPE_INT_GATE_32 | IDT_PRIV_0 | IDT_USED;
    x386->idt_entries[n].offset_high = (((uint32_t)idt_handler_bridge) >> 16) & 0xFFFF;
    x386->idt_entries[n].offset_low = ((uint32_t)idt_handler_bridge) & 0xFFFF;

    x386->idt_handlers[n] = noop_idt_handler;
  }

  asm volatile (
    "lidt [%0]"
    :
    : "r" (&x386->idt_descriptor)
  );
}

void noop_idt_handler() {
}

#if 0
static void encodeGdtEntry(uint8_t *target, struct GDT source) {
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && (source.limit & 0xFFF) != 0xFFF)) {
        kerror("You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // And... Type
    target[5] = source.type;
}
#endif

/*
Exceptions
	0                  Divide Error
	1                  Debug Exceptions
	2                  NMI Interrupt
	3                  Breakpoint
	4                  INTO Detected Overflow
	5                  BOUND Range Exceeded
	6                  Invalid Opcode
	7                  Coprocessor Not Available
	8                  Double Exception
	9                  Coprocessor Segment Overrun
	10                 Invalid Task State Segment
	11                 Segment Not Present
	12                 Stack Fault
	13                 General Protection
	14                 Page Fault
	15                 (reserved)
	16                 Coprocessor Error
	17-32              (reserved)

System registers

	EFLAGS
		IF (Interrupt-Enable Flag, bit 9)
		NT (Nested Task, bit 14)
		RF (Resume Flag, bit 16)
		TF (Trap Flag, bit 8)
		VM (Virtual 8086 Mode, bit 17)

	Memory-Management Registers
		GDTR    Global Descriptor Table Register
		LDTR    Local Descriptor Table Register
		IDTR    Interrupt Descriptor Table Register
		TR      Task Register

	Control Registers
		CR0 contains system control flags
			EM (Emulation, bit 2)
			ET (Extension Type, bit 4)
			MP (Math Present, bit 1)
			PE (Protection Enable, bit 0)
			PG (Paging, bit 31)
			TS (Task Switched, bit 3)
		CR2 is used for handling page faults when PG is set. The processor stores in CR2 the linear address that triggers the fault.
		CR3 is used when PG is set. CR3 enables the processor to locate the page table directory for the current task.

	Debug Registers

	Test Registers


*/
