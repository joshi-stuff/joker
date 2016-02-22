#include <stdint.h>
#include <duktape.h>

#include "multiboot.h"
#include "kernel.h"
#include "debug.h"
#include "twa.h"
#include "mmu.h"
#include "cpu.h"
#include "scr.h"

typedef struct _stack_frame_t {
  struct _stack_frame_t* next;
  void* ret;
} stack_frame_t;

extern uint8_t stack_bottom, stack_top, twa_bottom, twa_top;

static void *kernel_start = (void*) 0x100000;
static void *kernel_end = (void*) ((uint32_t) &stack_bottom - 1);
static void *stack_start = &stack_bottom;
static void *stack_end = (void*) ((uint32_t) &stack_top - 1);
static void *twa_start = &twa_bottom;
static void *twa_end = (void*) ((uint32_t) &twa_top - 1);

static size_t get_call_stack(size_t max_depth, void** return_addresses) {
  register stack_frame_t* fp asm("ebp");
  stack_frame_t* frame = fp;
  size_t depth;

  for (depth = 0; (depth < max_depth) && frame; depth++) {
    return_addresses[depth] = frame->ret;
    frame = frame->next;
  }

  return depth;
}

void k_panic(const char* msg) {
  scr_print("\nkernel panic: ");
  scr_print(msg);

  char stack[1024];

  void *addrs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  size_t count = get_call_stack(8, addrs);

  snprintf(stack, sizeof(stack), "\n\n"
      "call stack: %p %s\n"
      "            %p %s\n"
      "            %p %s\n"
      "            %p %s\n"
      "            %p %s\n"
      "            %p %s\n"
      "            %p %s\n"
      "            %p %s\n", addrs[0], dbg_symbol_at(addrs[0])->name, addrs[1],
      dbg_symbol_at(addrs[1])->name, addrs[2], dbg_symbol_at(addrs[2])->name,
      addrs[3], dbg_symbol_at(addrs[3])->name, addrs[4],
      dbg_symbol_at(addrs[4])->name, addrs[5], dbg_symbol_at(addrs[5])->name,
      addrs[6], dbg_symbol_at(addrs[6])->name, addrs[7],
      dbg_symbol_at(addrs[7])->name);

  scr_print(stack);

  asm volatile (
      "\tcli\n"
      ".Lhang:\n"
      "\thlt\n"
      "\tjmp .Lhang\n"
  );
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
  char msg[1024];

  snprintf(msg, sizeof(msg), "condition failed (%s:%d):\n\n  %s", file, line,
      condition);

  k_panic(msg);
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

  printf("kernel: booting joker");
  if ((mbi->flags & MBI_FLAG_CMDLINE) && *mbi->cmdline) {
    printf(" with arguments '%s'", mbi->cmdline);
  }
  printf("\n");
  printf("kernel: kernel is at [%p:%p]\n", kernel_start, kernel_end);
  printf("kernel: stack is at [%p:%p]\n", stack_start, stack_end);
  printf("kernel: twa is at [%p:%p]\n", twa_start, twa_end);

  printf("kernel: boot stage 1 started\n");

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
  printf("kernel: boot stage 2 started\n");

  dbg_init_2();

  twa_destroy();
  mmu_reclaim(twa_start, twa_end);
}

void main(multiboot_info_t* mbi, uint32_t magic) {
  k_init_1(mbi);
  k_init_2();

  // Run kernel
  printf("kernel: joker booted up and running\n");

}

/* http://yosefk.com/blog/getting-the-call-stack-without-a-frame-pointer.html */
