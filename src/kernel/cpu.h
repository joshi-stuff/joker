#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdint.h>

typedef uint8_t cpu_idt_number;
typedef void (*cpu_idt_handler)();

#define cpu_set_idt_gate(dev, n, handler)  DEV_CN(dev, set_idt_gate, n, handler)
#define cpu_clear_idt_gate(dev, n)         DEV_CN(dev, clear_idt_gate, n)

typedef struct _cpu {
  DEV_FN(void, set_idt_gate, cpu_idt_number n, cpu_idt_handler handler);
  DEV_FN(void, clear_idt_gate, cpu_idt_number n);
} cpu;

#endif
