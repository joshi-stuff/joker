#include <stdarg.h>
#include <stdint.h>

#include "multiboot.h"
#include "kernel.h"
#include "debug.h"
#include "twa.h"
#include "mmu.h"
#include "cpu.h"
#include "scr.h"
#include "helpers.h"

extern uint8_t stack_bottom, stack_top, twa_bottom, twa_top;

static void *kernel_start = (void*) 0x100000;
static void *kernel_end = (void*) ((uint32_t) &stack_bottom - 1);
static void *stack_start = &stack_bottom;
static void *stack_end = (void*) ((uint32_t) &stack_top - 1);
static void *twa_start = &twa_bottom;
static void *twa_end = (void*) ((uint32_t) &twa_top - 1);

void k_panic(const char* msg) {
  k_printf("\n\nkernel panic: %s\n\ncall stack:   ", msg);

  void *addrs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  size_t count = _get_call_stack(8, addrs);

  for (size_t i = 0; i < count; i++) {
    if (i > 0) {
      k_printf("              ");
    }
    k_printf("%p %s\n", addrs[i], dbg_symbol_at(addrs[i])->name);
  }

  asm volatile (
      "\tcli\n"
      ".Lhang:\n"
      "\thlt\n"
      "\tjmp .Lhang\n"
  );
}

void k_printf(const char* msg, ...) {
  va_list ap;

  va_start(ap, msg);

  while (*msg) {
    if (*msg == '%') {
      msg++;

      switch (*msg) {

      case '%': {
        scr_putc('%');
        break;
      }

      case 'c': {
        scr_putc(va_arg(ap, char));
        break;
      }

      case 's': {
        scr_print(va_arg(ap, char*));
        break;
      }

      case 'd': {
        char sz[32];
        _format_signed(va_arg(ap, int32_t), false, 0, sz);
        scr_print(sz);
        break;
      }

      case 'u': {
        char sz[32];
        _format_unsigned(va_arg(ap, uint32_t), false, 0, sz);
        scr_print(sz);
        break;
      }

      case 'w': {
        char sz[32];
        _format_unsigned(va_arg(ap, uint64_t), true, 16, sz);
        scr_print(sz);
        break;
      }

      case 'p':
      case 'x': {
        char sz[32];
        _format_unsigned(va_arg(ap, uint32_t), true, 8, sz);
        scr_print(sz);
        break;
      }

      case 'y': {
        char sz[32];
        _format_unsigned(va_arg(ap, uint16_t), true, 4, sz);
        scr_print(sz);
        break;
      }

      case 'z': {
        char sz[32];
        _format_unsigned(va_arg(ap, uint8_t), true, 2, sz);
        scr_print(sz);
        break;
      }

      default: {
        k_panic("invalid k_printf modifier");
      }
      }
    } else {
      scr_putc(*msg);
    }

    msg++;
  }

  va_end(ap);
}

void k_print(const char* msg) {
  scr_print(msg);
}

// TODO: these should be converted to brk and the functions should be implemented in user space
void* k_alloc(size_t size) {
  return mmu_alloc(size);
}

void* k_realloc(void* ptr, size_t size) {
  return mmu_realloc(ptr, size);
}

void k_free(void* ptr) {
  mmu_free(ptr);
}

void k_ensure_abort(const char* condition, const char* file, size_t line) {
  k_printf("condition failed at %s:%d\n");
  k_panic(condition);
}

/*
 * Stage 1: unstable memory boot stage
 *
 * Grab necessary data from memory without destroying it and init essential
 * devices carefully without modifying memory until we have a stable MMU.
 *
 * All dynamic memory needed in this stage must be retrieved from TWA.
 *
 * Upon finalization of stage 1, the multiboot_info_t is not guaranteed to be
 * valid.
 */
void k_init_1(multiboot_info_t* mbi) {
  scr_init_1();

  k_printf("kernel: booting joker");
  if ((mbi->flags & MBI_FLAG_CMDLINE) && *mbi->cmdline) {
    k_printf(" with arguments '%s'", mbi->cmdline);
  }
  k_printf("\n");
  k_printf("kernel: kernel is at [%p:%p]\n", kernel_start, kernel_end);
  k_printf("kernel: stack is at [%p:%p]\n", stack_start, stack_end);
  k_printf("kernel: twa is at [%p:%p]\n", twa_start, twa_end);

  k_printf("kernel: boot stage 1 started\n");

  cpu_init_1();

  twa_init_1(twa_start, twa_end);

  dbg_init_1((mbi->flags & MBI_FLAG_SYMS_ELF) ? &(mbi->syms.elf) : 0);

  cpu_state_t cpu_state;
  cpu_save_state(&cpu_state);

  k_ensure(mbi->flags & MBI_FLAG_MMAP);
  range_t lock_ranges[] = { { kernel_start, kernel_end }, { stack_start,
      stack_end }, { twa_start, twa_end }, { cpu_state.gdt.base,
      (void*) ((uint32_t) cpu_state.gdt.base + cpu_state.gdt.limit) } };
  mmu_init_1(mbi->mmap_addr, mbi->mmap_length, lock_ranges,
      sizeof(lock_ranges) / sizeof(range_t));
}

/*
 * Stage 2: stable memory boot stage
 *
 * Consolidate essential data temporarily stored in TWA to MMU and reclaim TWA
 * space for later use by MMU.
 *
 * Init the rest of devices.
 *
 * Upon finalization of stage 2, the TWA is no longer valid.
 */
void k_init_2() {
  k_printf("kernel: boot stage 2 started\n");

  dbg_init_2();

  twa_destroy();
  mmu_reclaim(twa_start, twa_end);
}

void main(multiboot_info_t* mbi, uint32_t magic) {
  k_init_1(mbi);
  k_init_2();

  // Run kernel
  k_printf("kernel: joker booted up and running\n");
}

/* http://yosefk.com/blog/getting-the-call-stack-without-a-frame-pointer.html */
