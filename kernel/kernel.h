#ifndef _KERNEL_H
#define _KERNEL_H

#include <stddef.h>

#define k_ensure(condition) if(!(condition)) k_ensure_abort(#condition, __FILE__, __LINE__)

void k_panic(const char* msg);
size_t k_get_call_stack(size_t max_depth, void** return_addresses);
void k_ensure_abort(const char* condition, const char* file, size_t line);

#endif
