#include <stdint.h>
#include <duktape.h>

#include "kernel.h"
#include "bios.h"
#include "mmu.h"
#include "multiboot.h"

extern uint8_t stack_bottom, stack_top;

static void *kernel_start = (void*) 0x100000;
static void *kernel_end = (void*) ((uint32_t) &stack_bottom - 1);
static void *stack_start = &stack_bottom;
static void *stack_end = &stack_top;

void main(multiboot_info* mbi, uint32_t magic) {

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
    panic("No memory map available from GRUB2");
  }
}

void panic(const char* msg) {
  bios_setcolor(COLOR_WHITE, COLOR_RED);
  bios_clrscr();
  printf("\n\n\n\n\n\n\n     ");
  printf("KERNEL PANIC: %s\n", msg);
  asm volatile (
      "\tcli\n"
      ".Lhang:\n"
      "\thlt\n"
      "\tjmp .Lhang\n"
  );
}

/*
 int print(duk_context *ctx) {
 int argc = duk_get_top(ctx);
 const char* msg = duk_to_string(ctx, -1);

 printf("%s", msg);

 return 0;
 }
 */

/*
 duk_context *ctx = duk_create_heap_default();

 duk_push_global_object(ctx);
 duk_push_c_function(ctx, print, DUK_VARARGS);
 duk_put_prop_string(ctx, -2, "print");
 duk_pop(ctx);

 duk_eval_string(ctx, "print('Hello world!\\n');");
 duk_pop(ctx);

 duk_destroy_heap(ctx);
 */

