#include "bios.h"
#include <duktape.h>
#include <stdint.h>

int print(duk_context *ctx) {
	int argc = duk_get_top(ctx);
	const char* msg = duk_to_string(ctx, -1);
	
	// OUTPUT TO VGA THE MESSAGE -> printf("%s", msg);
	
	return 0;
}

#define MAKE_COLOR(fg, bg)     ((uint8_t)((fg) | ((bg) << 4)))
#define MAKE_ENTRY(c, color)   ((uint16_t)(((uint16_t)c) | (((uint16_t)color) << 8)))
uint16_t* vga = (uint16_t*)0xB8000;

void main() {
	bios_init();
	
	bios_print("Welcome to Joker!\n");
	
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
}
