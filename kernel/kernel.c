#include "bios.h"
#include <duktape.h>
#include <stdint.h>

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

void main(void* mbd, uint32_t magic) {
	bios_init();
	bios_print("Welcome to Joker!\n");	
	
	char str[256];
	sprintf(str, "mbd=%08X magic=%08X", mbd, magic);
	bios_print("\n");
	bios_print("\n");
	bios_print(str);
	
}
