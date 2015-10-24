#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include <stdbool.h>

#include "incl/macros.h"

typedef const char* symbol;

typedef struct _device {
} device;

typedef struct _driver {
	symbol id;
	void (*load)();
	void (*unload)();
  void (*on_set_device)(symbol class, symbol id, device* dev);
  void (*on_remove_device)(symbol class, symbol id);
} driver;

void load_driver(driver* drv);
void unload_driver(driver* drv);

#define set_device(class, id, dev)  set_device_(#class, id, dev)
#define remove_device(class, id)    remove_device_(#class, id)
#define get_device(class, id)       ((class*)get_device_(#class, id))
void set_device_(symbol class, symbol id, void* dev);
void remove_device_(symbol class, symbol id);
device* get_device_(symbol class, symbol id);

void log(const char* who, const char* message);
void panic(const char* message);

bool is(symbol l, symbol r);

#endif
