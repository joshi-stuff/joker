#include "libc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int errno = 0;

void abort(void) {
}

void longjmp(jmp_buf env, int val) {
}

int setjmp(jmp_buf env) {
  return 0;
}
