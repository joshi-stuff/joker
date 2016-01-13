#include <stdbool.h>
#include <stddef.h>

#include "kernel.h"
#include "debug.h"
#include "string.h"
#include "console.h"

#define PRINT_REG32(dev, r) {\
  uint32_t val = 0x11223344;\
  asm volatile(\
    "mov %0, " r "\n"\
    : "=m" (val)\
  );\
  print_register(dev, r, &val, 4);\
}
#define PRINT_REG16(dev, r) {\
  uint16_t val = 0x1122;\
  asm volatile(\
    "mov %0, " r "\n"\
    : "=m" (val)\
  );\
  print_register(dev, r, &val, 2);\
}
#define PRINT_CR(dev, n) {\
  uint32_t val = 0x11223344;\
  asm volatile(\
    "mov %0, CR" #n "\n"\
    : "=r" (val)\
  );\
  print_register(dev, "CR" #n, &val, 4);\
}
#define PRINT_EFLAGS(dev) {\
  uint32_t val = 0x11223344;\
  asm volatile (\
    "pushfd\n"\
    "mov eax, [esp]\n"\
    "popfd\n"\
    "mov %0, eax\n"\
    : "=m" (val)\
    :\
    : "eax"\
  );\
  print_register(dev, "EFL", &val, 4);\
}
#define PRINT_GDT(dev) {\
  uint8_t val[6] = {0x11,0x22,0x33,0x44,0x55,0x66};\
  asm volatile (\
    "sgdt %0"\
    : "=m" (val)\
  );\
  print_register(dev, "GDT", val, 6);\
  print_gdt_contents(dev, (gdt_descriptor*)val);\
}
#define PRINT_IDT(dev) {\
  uint8_t val[6] = {0x11,0x22,0x33,0x44,0x55,0x66};\
  asm volatile(\
    "sidt %0"\
    : "=m" (val)\
  );\
  print_register(dev, "IDT", val, 6);\
}

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

DEFINE_DRIVER(debug);

DEFINE_DEVICE(debug, debug,
  uint16_t* trace_buffer;
  uint32_t  trace_offset;
  string*   str;
  console*  con;
);

static void trace(void* dev, char c);
static void dump_cpu(void* dev);

static void print_register(debug_debug* debug, const char* reg, void* val, uint32_t size);
static void print_gdt_contents(debug_debug* debug, gdt_descriptor* desc);

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

  PRINT_REG32(dev, "EAX");
  console_print(con, "  ");
  PRINT_REG32(dev, "EBX");
  console_print(con, "  ");
  PRINT_REG32(dev, "ECX");
  console_print(con, "  ");
  PRINT_REG32(dev, "EDX");
  console_print(con, "\n\n");

  PRINT_REG32(dev, "ESP");
  console_print(con, "  ");
  PRINT_REG32(dev, "EBP");
  console_print(con, "  ");
  PRINT_REG32(dev, "ESI");
  console_print(con, "  ");
  PRINT_REG32(dev, "EDI");
  console_print(con, "\n\n");

  PRINT_REG16(dev, "DS ");
  console_print(con, "      ");
  PRINT_REG16(dev, "ES ");
  console_print(con, "      ");
  PRINT_REG16(dev, "FS ");
  console_print(con, "      ");
  PRINT_REG16(dev, "GS ");
  console_print(con, "\n");
  PRINT_REG16(dev, "SS ");
  console_print(con, "      ");
  PRINT_REG16(dev, "CS ");
  console_print(con, "\n\n");

  PRINT_CR(dev, 0);
  console_print(con, "  ");
  PRINT_CR(dev, 2);
  console_print(con, "  ");
  PRINT_CR(dev, 3);
  console_print(con, "  ");
  PRINT_CR(dev, 4);
  console_print(con, "\n");
  PRINT_EFLAGS(dev);
  console_print(con, "\n\n");

  PRINT_GDT(dev);
  console_print(con, "\n");

  PRINT_IDT(dev);
  console_print(con, "\n\n");
}

void print_register(debug_debug* debug, const char* reg, void* _val, uint32_t size) {
  string* str = debug->str;
  console* con = debug->con;
  uint8_t* val = (uint8_t*)_val;
  char buffer[9];

  console_print(con, reg);
  console_print(con, " = 0x");
  for (uint32_t i = 0; i < size; i++) {
    string_to_hex(str, buffer, val[i], 2);
    console_print(con, buffer);
  }
}

void print_gdt_contents(debug_debug* debug, gdt_descriptor* desc) {
  string* str = debug->str;
  console* con = debug->con;

  gdt_entry* gdt_entries = (gdt_entry*)desc->base;
  uint32_t entries = desc->limit / 8;

  char buffer[9];

  console_print(con, "\n");
  for (uint32_t i=0; i < entries ; i++) {
    gdt_entry* e = gdt_entries+i;

    uint32_t limit = e->limit_low;
    limit |= (e->granularity & 0x0F) << 16;

    uint32_t base = e->base_low;
    base |= e->base_middle << 16;
    base |= e->base_high   << 24;

    uint32_t present     = (e->access & 0x80);
    uint32_t rw          = (e->access & 0x02);
    uint32_t exec        = (e->access & 0x08);
    uint32_t priv        = (e->access & 0x60) >> 5;
    uint32_t dc          = (e->access & 0x04);
    uint32_t granularity = (e->granularity & 0x80);
    uint32_t mode32      = (e->granularity & 0x40);

    console_print(con, "base:");
    string_to_hex(str, buffer, base, 8);
    console_print(con, buffer);
    console_print(con, " limit:");
    string_to_hex(str, buffer, limit, 5);
    console_print(con, buffer);
    console_print(con, " prs:");
    console_print(con, present ? "1" : "0");
    console_print(con, " rw:");
    console_print(con, rw ? "1" : "0");
    console_print(con, " exec:");
    console_print(con, exec ? "1" : "0");
    console_print(con, " mode:");
    console_print(con, mode32 ? "32" : "16");
    if (exec) {
      console_print(con, " conf:");
      console_print(con, dc ? "1 " : "0 ");
    }
    else {
      console_print(con, " grow:");
      console_print(con, dc ? "dn" : "up");
    }
    console_print(con, " priv:");
    string_to_hex(str, buffer, priv, 1);
    console_print(con, buffer);
    console_print(con, " gran:");
    console_print(con, granularity ? "4K" : "1b");
    console_print(con, "\n");
  }
}
