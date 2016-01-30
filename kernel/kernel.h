#ifndef _KERNEL_H
#define _KERNEL_H

#define ASSERT(condition) if(!(condition)) {printf("assert failed(%s:%d): %s", __FILE__, __LINE__, #condition);while(1);}

void panic(const char* msg);

#endif
