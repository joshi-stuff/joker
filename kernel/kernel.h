#ifndef _KERNEL_H
#define _KERNEL_H

#define k_ensure(condition) \
  if(!(condition)) {printf("assert failed(%s:%d): %s", __FILE__, __LINE__, #condition);while(1);}

void k_panic(const char* msg);

#endif
