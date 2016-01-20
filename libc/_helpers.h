#include <bios.h>

#define NOT_IMPLEMENTED(fn)   {bios_print("NOT IMPLEMENTED: " #fn "\n");return 0;}
#define NOT_IMPLEMENTED_V(fn) {bios_print("NOT IMPLEMENTED: " #fn "\n");}

#define DEBUG(var) {bios_print(#var "= ");bios_print(var);bios_print("\n");}

#define _is_num(c) (((c)-'0' <= 9) && ((c)-'0' >= 0))

void _reverse(char s[]);
int _to_int(char* begin, char* end);