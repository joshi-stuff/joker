#include "setjmp.h"
#include "_helpers.h"

int setjmp(jmp_buf env) {
  return 0;
}

void longjmp(jmp_buf env, int val) NOT_IMPLEMENTED_V(longjmp)
