/* BIOS **********************************************************************/

void bios_init();

/* BIOS: screen **************************************************************/

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_BROWN 14
#define COLOR_WHITE 15

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH  80

void bios_clrscr();
void bios_moveto(int row, int col);
void bios_setcolor(int fg, int bg);
void bios_print(const char* text);
void bios_scroll(int rows);
