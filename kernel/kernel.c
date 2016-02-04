#include <stdint.h>
#include <duktape.h>

#include "kernel.h"
#include "bios.h"
#include "debug.h"
#include "mmu.h"
#include "multiboot.h"

typedef struct _stack_frame_t {
  struct _stack_frame_t* next;
  void* ret;
} stack_frame_t;

extern uint8_t stack_bottom, stack_top;

static void *kernel_start = (void*) 0x100000;
static void *kernel_end = (void*) ((uint32_t) &stack_bottom - 1);
static void *stack_start = &stack_bottom;
static void *stack_end = &stack_top;

static int duk_print(duk_context *ctx) {
  int argc = duk_get_top(ctx);
  const char* msg = duk_to_string(ctx, -1);

  printf("%s", msg);

  return 0;
}

static void duk_test() {
  duk_context *ctx = duk_create_heap_default();

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, duk_print, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "print");
  duk_pop(ctx);

  duk_eval_string(ctx, "print('Hello world!\\n');");
  duk_pop(ctx);

  duk_destroy_heap(ctx);
}

void k_panic(const char* msg) {
  bios_print("\nkernel panic: ");
  bios_print(msg);
  asm volatile (
      "\tcli\n"
      ".Lhang:\n"
      "\thlt\n"
      "\tjmp .Lhang\n"
  );
}

size_t k_get_call_stack(size_t max_depth, void** return_addresses) {
  register stack_frame_t* fp asm("ebp");
  stack_frame_t* frame = fp;
  size_t depth;

  for (depth = 0; (depth < max_depth) && frame; depth++) {
    return_addresses[depth] = frame->ret;
    frame = frame->next;
  }

  return depth;
}

void k_ensure_abort(const char* condition, const char* file, size_t line) {
  char msg[1024];

  void *addrs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  size_t count = k_get_call_stack(8, addrs);

  snprintf(msg, sizeof(msg), "assert failed (%s:%d): %s\n"
      "kernel panic: call stack: %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n"
      "                          %p %s\n", file, line, condition, addrs[0],
      dbg_symbol_at(addrs[0])->name, addrs[1], dbg_symbol_at(addrs[1])->name,
      addrs[2], dbg_symbol_at(addrs[2])->name, addrs[3],
      dbg_symbol_at(addrs[3])->name, addrs[4], dbg_symbol_at(addrs[4])->name,
      addrs[5], dbg_symbol_at(addrs[5])->name, addrs[6],
      dbg_symbol_at(addrs[6])->name, addrs[7], dbg_symbol_at(addrs[7])->name);

  k_panic(msg);
}

void main(multiboot_info_t* mbi, uint32_t magic) {
  bios_init();

  printf("kernel: booting joker");
  if ((mbi->flags & MBI_FLAG_CMDLINE) && *mbi->cmdline) {
    printf(" with arguments '%s'", mbi->cmdline);
  }
  printf("\n");

  // Init memory map
  if (mbi->flags & MBI_FLAG_MMAP) {
    mmu_init(mbi->mmap_addr, mbi->mmap_length, kernel_start, kernel_end,
        stack_start, stack_end);
  } else {
    k_panic("no memory map available from GRUB2");
  }

  // Init debug
  if (mbi->flags & MBI_FLAG_SYMS_ELF) {
    dbg_init(&(mbi->syms.elf));
  } else {
    dbg_init(0);
  }

  duk_test();
}

/* http://yosefk.com/blog/getting-the-call-stack-without-a-frame-pointer.html */
