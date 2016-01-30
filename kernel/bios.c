#include <stdint.h>
#include "bios.h"


/* BIOS **********************************************************************/
#define outb(port, byte) asm volatile ("out %0, %1" : : "d" ((uint16_t)(port)), "a" ((uint8_t)(byte)))

void bios_init() {
	bios_clrscr();
}


/* BIOS: screen **************************************************************/
/* http://wiki.osdev.org/Text_Mode_Cursor */
/* http://www.osdever.net/bkerndev/Docs/printing.htm */

#define MAKE_COLOR(fg, bg)        ((uint8_t)((fg) | ((bg) << 4)))
#define MAKE_INDEX(row, col)      ((row) * SCREEN_WIDTH + (col))
#define MAKE_ENTRY(glyph, color)  ((uint16_t)(((uint16_t)glyph) | (((uint16_t)color) << 8)))

static uint16_t* _screen = (uint16_t*)0xB8000;
static int       _row    = 0;
static int       _col    = 0;
static uint8_t   _color  = MAKE_COLOR(COLOR_BLACK, COLOR_WHITE);

void bios_clrscr() {
  for (int row = 0; row < SCREEN_HEIGHT; row++) {
		for (int col = 0; col < SCREEN_WIDTH; col++) {
			_screen[MAKE_INDEX(row, col)] = MAKE_ENTRY(' ', _color);
		}
	}
  bios_moveto(0, 0);
}

void bios_moveto(int row, int col) {
  if ((row >= 0) && (row < SCREEN_HEIGHT)) {
    _row = row;
  }
	
  if ((col >= 0) && (col < SCREEN_WIDTH)) {
    _col = col;
  }
	
	uint16_t pos = MAKE_INDEX(_row, _col);

	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, pos);
}

void bios_setcolor(int fg, int bg) {
	_color = MAKE_COLOR(fg, bg);
}

void bios_print(const char* text) {
  while (*text) {
    switch (*text) {
      case '\n': {
        _col = 0;
        _row++;
        break;
      }
      default: {
        _screen[MAKE_INDEX(_row, _col)] = MAKE_ENTRY(*text, _color);
				_col++;

        if (_col == SCREEN_WIDTH) {
          _col = 0;
          _row++;
        }

        break;
      }
    }

    if (_row == SCREEN_HEIGHT) {
      bios_scroll(1);
      _row = SCREEN_HEIGHT - 1;
    }

    text++;
  }

  bios_moveto(_row, _col);
}

void bios_scroll(int rows) {
  if (rows > SCREEN_HEIGHT) {
    rows = SCREEN_HEIGHT;
  }

  for (int row = rows; row < SCREEN_HEIGHT; row++) {
		for (int col = 0; col < SCREEN_WIDTH; col++) {
			_screen[MAKE_INDEX(row-rows, col)] = _screen[MAKE_INDEX(row, col)];
		}
	}

  for (int row = SCREEN_HEIGHT - rows; row < SCREEN_HEIGHT; row++) {
    for (int col = 0; col < SCREEN_WIDTH; col++) {
			_screen[MAKE_INDEX(row, col)] = MAKE_ENTRY(' ', _color);
		}
  }	
}
