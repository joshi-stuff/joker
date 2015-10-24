#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "console.h"
#include "debug.h"

typedef struct _device_entry {
  symbol  class;
  symbol  id;
  device* dev;
} device_entry;

/* TODO: this should be retrieved from a file in boot partition or file system */
extern driver* debug_driver;
extern driver* i386_driver;
extern driver* string_driver;
extern driver* vga_driver;

/* TODO: this should be a dynamic list living in the heap */
static uint32_t drivers_count = 0;
static driver* drivers[256];

/* TODO: this should be a dynamic map living in the heap */
static uint32_t device_entries_count = 0;
static device_entry device_entries[256];

static void loop_forever();

static console* con;

//  *((uint16_t*)0xB8000) = (uint16_t)con;
void main() {
  load_driver(string_driver);

  load_driver(vga_driver);
  con = get_device(console, NULL);
  console_print(con, "Hola");

  load_driver(debug_driver);
  debug* dbg = get_device(debug, NULL);
  debug_dump_cpu(dbg);

  //load_driver(i386_driver);

  loop_forever();
}

void load_driver(driver* drv) {
  /* Register and load driver */
  drivers[drivers_count++] = drv;
  drv->load();

  /* Send set_device events for all existing devices */
  for (uint32_t i=0; i<device_entries_count; i++) {
    device_entry* entry = &device_entries[i];
    drv->on_set_device(entry->class, entry->id, entry->dev);
  }
}

void unload_driver(driver* drv) {
  /* Find and unload driver */
  for (uint32_t i=0; i<drivers_count; i++) {
    if (drivers[i] == drv) {
      drivers[i] = drivers[--drivers_count];
      drv->unload();
      return;
    }
  }
}

void set_device_(symbol class, symbol id, void* dev) {
  /* If already installed ignore request */
  if (get_device_(class, id) == dev) {
    return;
  }

  /* Remove device in case it is registered */
  remove_device_(class, id);

  /* Register device */
  device_entry *entry = &device_entries[device_entries_count];
  entry->class = class;
  entry->id = id;
  entry->dev = dev;
  device_entries_count++;

  /* Send set_device event to all existing drivers */
  for (uint32_t i=0; i<drivers_count; i++) {
    drivers[i]->on_set_device(class, id, dev);
  }
}

void remove_device_(symbol class, symbol id) {
  /* Find device ... */
  for (uint32_t i=0; i<device_entries_count; i++) {
    device_entry* entry = &device_entries[i];

    if (is(entry->class, class) && is(entry->id, id)) {
      /* ...and unregister it */
      device_entries[i] = device_entries[device_entries_count--];

      /* Send remove_device events to all existing drivers */
      for (uint32_t i=0; i<drivers_count; i++) {
        drivers[i]->on_remove_device(entry->class, entry->id);
      }

      /* Finish */
      return;
    }
  }
}

device* get_device_(symbol class, symbol id) {
  for (uint32_t i=0; i<device_entries_count; i++) {
    device_entry *entry = &device_entries[i];

    if (is(entry->class, class) && is(entry->id, id)) {
      return entry->dev;
    }
  }

  return NULL;
}

void log(const char* who, const char* message) {
  /* TODO: write logs when we have file system */
}

void panic(const char* message) {
  console_clear(con);
  console_print(con, "** KERNEL PANIC **\n\n");
  console_print(con, message);

  CLI;
  loop_forever();
}

bool is(symbol left, symbol right) {
  if (left == right) {
    return true;
  }

  while (*left && *right && *left == *right) {
    left++;
    right++;
  }

  return *left == *right;
}

void __attribute__ ((noinline)) loop_forever() {
  asm volatile (
		".loop_forever:\n"
		"		hlt\n"
		"		jmp .loop_forever\n"
	);
}
