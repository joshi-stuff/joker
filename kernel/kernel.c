#include <stdint.h>
#include <stdio.h>
#include <duktape.h>

#include "bios.h"
#include "multiboot.h"

/*
 * sizeof(char)      = 1
 * sizeof(short)     = 2
 * sizeof(int)       = 4
 * sizeof(long)      = 4
 * sizeof(long long) = 8
 * intmax_t          = 8
 * size_t            = 4
 * ptrdiff_t         = 4
 */

int print(duk_context *ctx) {
  int argc = duk_get_top(ctx);
  const char* msg = duk_to_string(ctx, -1);

  printf("%s", msg);

  return 0;
}

void main(multiboot_info* mbi, uint32_t magic) {
  bios_init();

  printf("Welcome to Joker!\n");
  /*
   printf("\n");

   if (mbi->flags & MBI_FLAG_BOOT_DEVICE) {
   printf("boot_device = %X\n", mbi->boot_device);
   }

   if (mbi->flags & MBI_FLAG_CMDLINE) {
   printf("cmdline = %s\n", mbi->cmdline);
   }

   if (mbi->flags & MBI_FLAG_MODS) {
   printf("mods_count = %u\n", mbi->mods_count);
   printf("mods_addr = %X\n", mbi->mods_addr);
   }

   if (mbi->flags & MBI_FLAG_MMAP) {
   printf("mmap_length = %u\n", mbi->mmap_length);

   mmap_entry* mma = mbi->mmap_addr;
   while ((((char*) mma) - ((char*) mbi->mmap_addr)) < mbi->mmap_length) {
   printf("  base_addr = %llX, end_addr = %llX, length = %lld, type=%X\n",
   mma->base_addr, mma->base_addr + mma->length, mma->length, mma->type);

   mma = (mmap_entry*) (((char*) mma) + mma->entry_size + 4);
   }
   }

   if (mbi->flags & MBI_FLAG_DRIVES) {
   printf("drives_length = %u\n", mbi->drives_length);
   printf("drives_addr = %X\n", mbi->drives_addr);
   }

   if (mbi->flags & MBI_FLAG_CONFIG_TABLE) {
   printf("config_table = %X\n", mbi->config_table);
   }

   if (mbi->flags & MBI_FLAG_BOOT_LOADER_NAME) {
   printf("boot_loader_name = %s\n", mbi->boot_loader_name);
   }

   if (mbi->flags & MBI_FLAG_APM_TABLE) {
   printf("apm_table = %X\n", mbi->apm_table);
   }

   if (mbi->flags & MBI_FLAG_VBE) {
   printf("vbe_control_info = %X\n", mbi->vbe_control_info);
   printf("vbe_mode_info = %X\n", mbi->vbe_mode_info);
   printf("vbe_mode = %hX\n", mbi->vbe_mode);
   printf("vbe_interface_seg = %hX\n", mbi->vbe_mode);
   printf("vbe_interface_off = %hX\n", mbi->vbe_interface_off);
   printf("vbe_interface_len = %X\n", mbi->vbe_interface_len);
   }
   */
  duk_context *ctx = duk_create_heap_default();

  /*
  duk_push_global_object(ctx);
  duk_push_c_function(ctx, print, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "print");
  duk_pop(ctx);

  duk_eval_string(ctx, "print('Hello world!\\n');");
  duk_pop(ctx);

  duk_destroy_heap(ctx);
  */
}
