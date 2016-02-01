#include <stdint.h>

#include "bios.h"

static uint16_t* screen_buffer = (uint16_t*) 0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t current_color;

#define outb(port, byte)          asm volatile ("out %0, %1" : : "d" ((uint16_t)(port)), "a" ((uint8_t)(byte)))
#define make_color(fg, bg)        ((uint8_t)((fg) | ((bg) << 4)))
#define make_index(row, col)      ((row) * SCREEN_WIDTH + (col))
#define make_entry(glyph, color)  ((uint16_t)(((uint16_t)glyph) | (((uint16_t)color) << 8)))

void bios_init() {
  current_color = make_color(COLOR_BLACK, COLOR_WHITE);
  bios_clrscr();
}

void bios_clrscr() {
  for (int row = 0; row < SCREEN_HEIGHT; row++) {
    for (int col = 0; col < SCREEN_WIDTH; col++) {
      screen_buffer[make_index(row, col)] = make_entry(' ', current_color);
    }
  }
  bios_moveto(0, 0);
}

void bios_moveto(int row, int col) {
  if ((row >= 0) && (row < SCREEN_HEIGHT)) {
    cursor_row = row;
  }

  if ((col >= 0) && (col < SCREEN_WIDTH)) {
    cursor_col = col;
  }

  uint16_t pos = make_index(cursor_row, cursor_col);

  outb(0x3D4, 14);
  outb(0x3D5, pos >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, pos);
}

void bios_setcolor(int fg, int bg) {
  current_color = make_color(fg, bg);
}

void bios_print(const char* text) {
  while (*text) {
    switch (*text) {
    case '\n': {
      cursor_col = 0;
      cursor_row++;
      break;
    }
    default: {
      screen_buffer[make_index(cursor_row, cursor_col)] = make_entry(*text,
          current_color);
      cursor_col++;

      if (cursor_col == SCREEN_WIDTH) {
        cursor_col = 0;
        cursor_row++;
      }

      break;
    }
    }

    if (cursor_row == SCREEN_HEIGHT) {
      bios_scroll(1);
      cursor_row = SCREEN_HEIGHT - 1;
    }

    text++;
  }

  bios_moveto(cursor_row, cursor_col);
}

void bios_scroll(int rows) {
  if (rows > SCREEN_HEIGHT) {
    rows = SCREEN_HEIGHT;
  }

  for (int row = rows; row < SCREEN_HEIGHT; row++) {
    for (int col = 0; col < SCREEN_WIDTH; col++) {
      screen_buffer[make_index(row - rows, col)] = screen_buffer[make_index(row,
          col)];
    }
  }

  for (int row = SCREEN_HEIGHT - rows; row < SCREEN_HEIGHT; row++) {
    for (int col = 0; col < SCREEN_WIDTH; col++) {
      screen_buffer[make_index(row, col)] = make_entry(' ', current_color);
    }
  }
}

/* BIOS: screen **************************************************************/
/* http://wiki.osdev.org/Text_Mode_Cursor */
/* http://www.osdever.net/bkerndev/Docs/printing.htm */
