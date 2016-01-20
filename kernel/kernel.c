#include <duktape.h>
#include <stdint.h>
#include <stdio.h>
#include "bios.h"
#include "multiboot.h"

/*
int print(duk_context *ctx) {
	int argc = duk_get_top(ctx);
	const char* msg = duk_to_string(ctx, -1);
	
	bios_print(msg);
	
	return 0;
}

duk_context *ctx = duk_create_heap_default();

duk_push_global_object(ctx);
duk_push_c_function(ctx, print, DUK_VARARGS);
duk_put_prop_string(ctx, -2, "print");
duk_pop(ctx); 

duk_eval_string(ctx, "print('Hello world!\\n');");
duk_pop(ctx);

duk_destroy_heap(ctx);
*/

void main(multiboot_info* mbi, uint32_t magic) {
	bios_init();
	
	printf("Welcome to Joker!\n");	
	printf("\n");
	printf("mem_lower = %X\n", mbi->mem_lower);
	printf("mem_upper = %X\n", mbi->mem_upper);
	printf("\n");
	printf("mmap_length = %X\n", mbi->mmap_length);
	printf("mmap_addr = %X\n", mbi->mmap_addr);
	
	mmap_entry* mma = mbi->mmap_addr;
	printf("\n");
	printf("entry_size = %X\n", mma->entry_size);
	printf("base_addr = %llX\n", mma->base_addr);
	printf("length = %llX\n", mma->length);
	printf("type = %X\n", mma->type);
}
