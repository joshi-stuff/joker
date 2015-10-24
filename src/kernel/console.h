#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <stdint.h>

typedef enum _console_color {
	CONSOLE_COLOR_BLACK = 0,
	CONSOLE_COLOR_BLUE = 1,
	CONSOLE_COLOR_GREEN = 2,
	CONSOLE_COLOR_CYAN = 3,
	CONSOLE_COLOR_RED = 4,
	CONSOLE_COLOR_MAGENTA = 5,
	CONSOLE_COLOR_BROWN = 6,
	CONSOLE_COLOR_LIGHT_GREY = 7,
	CONSOLE_COLOR_DARK_GREY = 8,
	CONSOLE_COLOR_LIGHT_BLUE = 9,
	CONSOLE_COLOR_LIGHT_GREEN = 10,
	CONSOLE_COLOR_LIGHT_CYAN = 11,
	CONSOLE_COLOR_LIGHT_RED = 12,
	CONSOLE_COLOR_LIGHT_MAGENTA = 13,
	CONSOLE_COLOR_LIGHT_BROWN = 14,
	CONSOLE_COLOR_WHITE = 15,
} console_color;

#define console_clear(dev)                     DEV_C0(dev, clear)
#define console_move_to(dev, x, y)             DEV_CN(dev, move_to, x, y)
#define console_set_color(dev, fg, bg)         DEV_CN(dev, set_color, fg, bg)
#define console_print(dev, text)               DEV_CN(dev, print, text)
#define console_print10(dev, value)            DEV_CN(dev, print10, value)
#define console_print16(dev, value)            DEV_CN(dev, print16, value)
#define console_dump(dev, label, buffer, len)  DEV_CN(dev, dump, label, buffer, len)
#define console_get_x(dev)                     DEV_C0(dev, get_x)
#define console_get_y(dev)                     DEV_C0(dev, get_y)
#define console_get_fg(dev)                    DEV_C0(dev, get_fg)
#define console_get_bg(dev)                    DEV_C0(dev, get_bg)
#define console_scroll(dev, lines)             DEV_CN(dev, scroll, lines)

typedef struct _console {
  DEV_F0(void, clear);
  DEV_FN(void, move_to, uint32_t x, uint32_t y);
  DEV_FN(void, set_color, console_color fg, console_color bg);
  DEV_FN(void, print, const char* text);
  DEV_FN(void, dump, const char* label, void* buffer, uint32_t len);
  DEV_F0(uint32_t, get_x);
  DEV_F0(uint32_t, get_y);
  DEV_F0(console_color, get_fg);
  DEV_F0(console_color, get_bg);
  DEV_FN(void, scroll, uint32_t lines);
} console;

#endif
