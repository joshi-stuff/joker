#ifndef _INCLUDE_KERNEL_H
#define _INCLUDE_KERNEL_H

#include <stddef.h>

void k_panic(const char* msg);

// Only %%, %c, %s, %d, %u, %p and (%w, %x, %y, %z) are allowed
// %w=64bit, %p=%x=32bit, %y=16bit, %z=8bit
void k_printf(const char* msg, ...);

// TODO: these should be converted to brk and the functions should be implemented in user space
void* k_alloc(size_t size);
void* k_realloc(void* ptr, size_t size);
void k_free(void* ptr);

#endif
