#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdint.h>

#define debug_trace(dev, c)  DEV_CN(dev, trace, c)
#define debug_dump_cpu(dev)  DEV_C0(dev, dump_cpu)

typedef struct _debug {
  DEV_FN(void, trace, char c);
  DEV_F0(void, dump_cpu);
} debug;

#endif
