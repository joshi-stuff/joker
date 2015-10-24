#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "console.h"
#include "string.h"

#define MAKE_COLOR(fg, bg)     ((uint8_t)((fg) | ((bg) << 4)))
#define MAKE_INDEX(dev, x, y)  ((y) * dev->width + (x))
#define MAKE_ENTRY(c, color)   ((uint16_t)(((uint16_t)c) | (((uint16_t)color) << 8)))

DEFINE_DRIVER(vga);

DEFINE_DEVICE(vga, console,
  uint16_t* buffer;
  uint32_t  width;
  uint32_t  height;
  uint32_t  x;
  uint32_t  y;
  uint8_t   color;
  string*   str;
);

static void clear(void* dev);
static void move_to(void* dev, uint32_t x, uint32_t y);
static void set_color(void* dev, console_color fg, console_color bg);
static void print(void* dev, const char* text);
static void dump(void* dev, const char* label, void* buffer, uint32_t len);
static uint32_t get_x(void* dev);
static uint32_t get_y(void* dev);
static console_color get_fg(void* dev);
static console_color get_bg(void* dev);
static void scroll(void* dev, uint32_t lines);

/* TODO: this should be in the heap */
static vga_console default_vga_console = {
  {
    clear, move_to, set_color, print, dump, get_x, get_y, get_fg, get_bg, scroll
  },
  (uint16_t*)0xB8000, 80, 25, 0, 0,
  MAKE_COLOR(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK),
  NULL
};

void load() {
  clear(&default_vga_console);
}

void unload() {
  remove_device(console, NULL);
  clear(&default_vga_console);
}

void on_set_device(symbol class, symbol id, device* dev) {
  SET_DEVICE_DEP(default_vga_console.str, string, NULL);
  CHECK_DEVICE_DEPS(
    console, NULL, &default_vga_console,
    default_vga_console.str);
}

void on_remove_device(symbol class, symbol id) {
  REMOVE_DEVICE_DEP(default_vga_console.str, string, NULL);
  CHECK_DEVICE_DEPS(
    console, NULL, &default_vga_console,
    default_vga_console.str);
}

void clear(void* dev) {
  DEV_LOAD(vga, console);

  for (uint32_t y = 0; y < console->height; y++) {
		for (uint32_t x = 0; x < console->width; x++) {
			console->buffer[MAKE_INDEX(console, x, y)] = MAKE_ENTRY(' ', console->color);
		}
	}
  console->x = 0;
  console->y = 0;
}

void move_to(void* dev, uint32_t x, uint32_t y) {
  DEV_LOAD(vga, console);

  if (x>0 && x<console->width) {
    console->x = x;
  }
  if (y>0 && y<console->height) {
    console->y = y;
  }
}

void set_color(void* dev, console_color fg, console_color bg) {
  DEV_LOAD(vga, console);

  console->color = MAKE_COLOR(fg, bg);
}

void print(void* dev, const char* text) {
  DEV_LOAD(vga, console);

  while (*text) {
    switch (*text) {
      case '\n': {
        console->x = 0;
        console->y++;
        break;
      }
      default: {
        console->buffer[MAKE_INDEX(console, console->x, console->y)] =
          MAKE_ENTRY(*text, console->color);

        console->x++;
        if (console->x == console->width) {
          console->x = 0;
          console->y++;
        }

        break;
      }
    }

    if (console->y == console->height) {
      scroll(dev, 1);
      console->y = console->height - 1;
    }

    text++;
  }

  // TODO: update cursor position
}

void dump(void* dev, const char* label, void* buffer, uint32_t len) {
  DEV_LOAD(vga, console);

  print(dev, label);
  print(dev, ":\n");

  uint8_t* p = (uint8_t*)buffer;
  char cbuf[2] = {0, 0};
  uint32_t i;

  for (i=0; i<len; i++) {
    if((i != 0) && (i%16 == 0)) {
        print(dev, " ");
        for (uint32_t j=16; j>0; j--) {
          cbuf[0] = p[i-j];
          print(dev, cbuf);
        }
        print(dev, "\n");
    }

    char hex_string[9];
    string_to_hex(console->str, hex_string, p[i], 2);
    print(dev, hex_string);
    print(dev, " ");
  }

  for (uint32_t j=(i%16); j<16; j++) {
    print(dev, "   ");
  }
  print(dev, " ");
  for (uint32_t j=(i%16); j>0; j--) {
    cbuf[0] = p[i-j];
    print(dev, cbuf);
  }
  print(dev, "\n");
}

uint32_t get_x(void* dev) {
  DEV_LOAD(vga, console);

  return console->x;
}

uint32_t get_y(void* dev) {
  DEV_LOAD(vga, console);

  return console->y;
}

console_color get_fg(void* dev) {
  DEV_LOAD(vga, console);

  return console->color & 0x0F;
}

console_color get_bg(void* dev) {
  DEV_LOAD(vga, console);

  return (console->color & 0xF0) >> 4;
}

void scroll(void* dev, uint32_t lines) {
  DEV_LOAD(vga, console);

  if (lines > console->height) {
    lines = console->height;
  }

  for (uint32_t y = lines; y < console->height; y++) {
		for (uint32_t x = 0; x < console->width; x++) {
			console->buffer[MAKE_INDEX(console, x, y-lines)] = console->buffer[MAKE_INDEX(console, x, y)];
		}
	}

  for (uint32_t y = console->height - lines; y < console->height; y++) {
    for (uint32_t x = 0; x < console->width; x++) {
			console->buffer[MAKE_INDEX(console, x, y)] = MAKE_ENTRY(' ', console->color);
		}
  }
}

#if 0
// From: http://www.osdever.net/bkerndev/Docs/printing.htm
/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csrcon->y * 80 + csrcon->x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}
*/
#endif
