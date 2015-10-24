#include "duktape.h"

int print(duk_context *ctx) {
	int argc = duk_get_top(ctx);
	const char* msg = duk_to_string(ctx, -1);
	
	// OUTPUT TO VGA THE MESSAGE -> printf("%s", msg);
	
	return 0;
}

 int main(int argc, char *argv[]) {
	duk_context *ctx = duk_create_heap_default();

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, print, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "print");
	duk_pop(ctx); 

	duk_eval_string(ctx, "print('Hello world!\\n');");
	duk_pop(ctx);

	duk_destroy_heap(ctx);

	return 0;
}
