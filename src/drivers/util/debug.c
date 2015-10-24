#include <stdbool.h>
#include <stddef.h>

#include "kernel.h"
#include "debug.h"
#include "string.h"
#include "console.h"

#define PRINT_REGISTER(dev, r) {\
  uint32_t val = 0x12344321;\
  asm volatile("mov %0, " r "\n" : "=rm" (val));\
  print_register(dev, r, val);\
}
#define PRINT_EFLAGS(dev) {\
  uint32_t val = 0x12344321;\
  asm volatile (\
    "pushfd\n"\
    "mov eax, [esp]\n"\
    "popfd\n"\
    "mov %0, eax\n"\
    : "=rm" (val)\
    :\
    : "eax"\
  );\
  print_register(dev, "EFL", val);\
}
#define PRINT_GDT(dev) {\
  uint8_t val[6] = {0x11,0x22,0x33,0x44,0x55,0x66};\
  asm volatile (\
    "sgdt %0"\
    : "=m" (val)\
    :\
    : "memory"\
  );\
  print_register(dev, "GDT", *(uint32_t*)val);\
  print_register(dev, "GDT", *(uint32_t*)(val+2));\
}
#define PRINT_IDT(dev) {\
  uint32_t val = 0x12344321;\
  asm volatile(\
    "sidt [esp]"\
  );\
  print_register(dev, "IDT", val);\
}

DEFINE_DRIVER(debug);

DEFINE_DEVICE(debug, debug,
  uint16_t* trace_buffer;
  uint32_t  trace_offset;
  string*   str;
  console*  con;
);

static void trace(void* dev, char c);
static void dump_cpu(void* dev);

static void print_register(debug_debug* debug, const char* reg, uint32_t val);

/* TODO: this should be in the heap */
static debug_debug default_debug_debug = {
  {
    trace, dump_cpu
  },
  (uint16_t*)0xB8000,
  0,
  NULL,
  NULL
};

void load() {
}

void unload() {
  remove_device(debug, NULL);
}

void on_set_device(symbol class, symbol id, device* dev) {
  SET_DEVICE_DEP(default_debug_debug.str, string, NULL);
  SET_DEVICE_DEP(default_debug_debug.con, console, NULL);
  CHECK_DEVICE_DEPS(
    debug, NULL, &default_debug_debug,
    default_debug_debug.str && default_debug_debug.con);
}

void on_remove_device(symbol class, symbol id) {
  REMOVE_DEVICE_DEP(default_debug_debug.str, string, NULL);
  REMOVE_DEVICE_DEP(default_debug_debug.con, console, NULL);
  CHECK_DEVICE_DEPS(
    debug, NULL, &default_debug_debug,
    default_debug_debug.str && default_debug_debug.con);
}

void trace(void* dev, char c) {
  DEV_LOAD(debug, debug);

  debug->trace_buffer[debug->trace_offset++] = 0x0F00 | c;
}

void dump_cpu(void* dev) {
  DEV_LOAD(debug, debug);

  console* con = debug->con;

  console_clear(con);

  PRINT_REGISTER(dev, "CR0");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "CR2");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "CR3");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "CR4");
  console_print(con, "\n");

  PRINT_EFLAGS(dev);
  console_print(con, "\n\n");

  PRINT_REGISTER(dev, "DS ");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "ES ");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "FS ");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "GS ");
  console_print(con, "\n");

  PRINT_REGISTER(dev, "SS ");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "ESP");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "EBP");
  console_print(con, "\n");

  PRINT_REGISTER(dev, "CS ");
  console_print(con, "\n\n");

  PRINT_REGISTER(dev, "EAX");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "EBX");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "ECX");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "EDX");
  console_print(con, "\n");

  PRINT_REGISTER(dev, "ESI");
  console_print(con, "  ");
  PRINT_REGISTER(dev, "EDI");
  console_print(con, "  ");
  console_print(con, "\n\n");

  PRINT_GDT(dev);
  console_print(con, "  ");
  PRINT_IDT(dev);
  console_print(con, "  ");
}

void print_register(debug_debug* debug, const char* reg, uint32_t val) {
  string* str = debug->str;
  console* con = debug->con;
  char buffer[9];

  console_print(con, reg);
  console_print(con, " = 0x");
  string_to_hex(str, buffer, val, true);
  console_print(con, buffer);
}
