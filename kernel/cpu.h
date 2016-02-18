#ifndef _CPU_H
#define _CPU_H

#define GDT_GRANULARITY_BYTE 0x00
#define GDT_GRANULARITY_PAGE 0x80
#define GDT_MODE_16          0x00
#define GDT_MODE_32          0x40
#define GDT_PRESENT          0x80
#define GDT_MISSING          0x00
#define GDT_PRIV_3           0x60
#define GDT_PRIV_2           0x40
#define GDT_PRIV_1           0x20
#define GDT_PRIV_0           0x00
#define GDT_EXECUTABLE       0x08
#define GDT_DATA             0x00
#define GDT_DIR_DOWN         0x04
#define GDT_DIR_UP           0x00
#define GDT_CONFORMING       0x04
#define GDT_NON_CONFORMING   0x00
#define GDT_READABLE         0x02
#define GDT_NON_READABLE     0x00
#define GDT_WRITABLE         0x02
#define GDT_NON_WRITABLE     0x00
#define GDT_DIRTY            0x01
#define GDT_CLEAN            0x00

#define IDT_PRESENT          0x80
#define IDT_MISSING          0x00
#define IDT_PRIV_3           0x60
#define IDT_PRIV_2           0x40
#define IDT_PRIV_1           0x20
#define IDT_PRIV_0           0x00
#define IDT_TASK_GATE_32     0x05
#define IDT_INT_GATE_16      0x06
#define IDT_TRAP_GATE_16     0x07
#define IDT_INT_GATE_32      0x0E
#define IDT_TRAP_GATE_32     0x0F

typedef struct __attribute__((packed)) _gdt_t {
  uint16_t limit;
  void* base;
} gdt_t;

typedef struct __attribute__((packed)) _idt_t {
  uint16_t limit;
  void* base;
} idt_t;

typedef struct __attribute__((packed)) _gdt_entry_t {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t limit_and_flags;
  uint8_t base_high;
} gdt_entry_t;

typedef struct __attribute__((packed)) _idt_entry_t {
  uint16_t offset_low;  // offset bits 0..15
  uint16_t selector;    // a code segment selector in GDT or LDT
  uint8_t zero;         // unused, set to 0
  uint8_t flags;        // type and attributes, see below
  uint16_t offset_high; // offset bits 16..31
} idt_entry_t;

typedef struct _cpu_state_t {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;

  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;

  uint16_t ds;
  uint16_t es;
  uint16_t fs;
  uint16_t gs;
  uint16_t ss;
  uint16_t cs;

  uint32_t eflags;

  uint32_t cr0;
  uint32_t cr2;
  uint32_t cr3;
  uint32_t cr4;

  gdt_t gdt;
  idt_t idt;
} cpu_state_t;

#define cpu_gdt_entry_base(e) (((uint32_t)(e)->base_low) | ((e)->base_middle << 16) | ((e)->base_high << 24))
#define cpu_gdt_entry_limit(e) (((uint32_t)(e)->limit_low) | (((e)->limit_and_flags&0x0F) << 16))
#define cpu_gdt_entry_granularity(e) ((e)->limit_and_flags & 0x80)
#define cpu_gdt_entry_mode(e) ((e)->limit_and_flags & 0x40)
#define cpu_gdt_entry_present(e) ((e)->access & 0x80)
#define cpu_gdt_entry_privilege(e) ((e)->access & 0x60)
#define cpu_gdt_entry_executable(e) ((e)->access & 0x08)
#define cpu_gdt_entry_dc(e) ((e)->access & 0x04)
#define cpu_gdt_entry_rw(e) ((e)->access & 0x02)
#define cpu_gdt_entry_dirty(e) ((e)->access & 0x01)

#define cpu_idt_entry_selector(e) ((e)->selector)
#define cpu_idt_entry_offset(e) (((uint32_t)((e)->offset_low)) | (((uint32_t)((e)->offset_high))<<16))
#define cpu_idt_entry_present(e) ((e)->flags & 0x80)
#define cpu_idt_entry_privilege(e) ((e)->flags & 0x60)
#define cpu_idt_entry_type(e) ((e)->flags & 0x0F)

// TODO: this should avoid modifying the state of CPU?
#define cpu_save_state(cpu_state) asm volatile(\
    "mov %0, eax\n"\
    "mov %1, ebx\n"\
    "mov %2, ecx\n"\
    "mov %3, edx\n"\
    "mov %4, esp\n"\
    "mov %5, ebp\n"\
    "mov %6, esi\n"\
    "mov %7, edi\n"\
    "mov %8, es\n"\
    "mov %9, ds\n"\
    "mov %10, fs\n"\
    "mov %11, gs\n"\
    "mov %12, ss\n"\
    "mov %13, cs\n"\
    "mov %14, cr0\n"\
    "mov %15, cr2\n"\
    "mov %16, cr3\n"\
    "mov %17, cr4\n"\
    "sgdt %18\n"\
    "sidt %19\n"\
    "pushfd\n"\
    "mov eax, [esp]\n"\
    "popfd\n"\
    "mov %20, eax\n"\
    :\
    "=m" ((cpu_state)->eax),\
    "=m" ((cpu_state)->ebx),\
    "=m" ((cpu_state)->ecx),\
    "=m" ((cpu_state)->edx),\
    "=m" ((cpu_state)->esp),\
    "=m" ((cpu_state)->ebp),\
    "=m" ((cpu_state)->esi),\
    "=m" ((cpu_state)->edi),\
    "=m" ((cpu_state)->es),\
    "=m" ((cpu_state)->ds),\
    "=m" ((cpu_state)->fs),\
    "=m" ((cpu_state)->gs),\
    "=m" ((cpu_state)->ss),\
    "=m" ((cpu_state)->cs),\
    "=r" ((cpu_state)->cr0),\
    "=r" ((cpu_state)->cr2),\
    "=r" ((cpu_state)->cr3),\
    "=r" ((cpu_state)->cr4),\
    "=m" ((cpu_state)->gdt),\
    "=m" ((cpu_state)->idt),\
    "=m" ((cpu_state)->eflags)\
    ::\
    "eax"\
  )

void cpu_init_1();

void cpu_set_idt_gate(uint8_t num, uint16_t selector, void* ptr,
    uint8_t privilege, uint8_t type);

void cpu_dump_registers();
void cpu_dump_gdt();
void cpu_dump_idt();

#endif
