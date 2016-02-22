#ifndef _KERNEL_H
#define _KERNEL_H

#include <stddef.h>

#include "include/kernel.h"

#define k_ensure(condition) if(!(condition)) k_ensure_abort(#condition, __FILE__, __LINE__)

void k_ensure_abort(const char* condition, const char* file, size_t line);

#endif
