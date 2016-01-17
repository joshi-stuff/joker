#include <bios.h>

#define NOT_IMPLEMENTED(fn)   {bios_print("NOT IMPLEMENTED: " #fn "\n");return 0;}
#define NOT_IMPLEMENTED_V(fn) {bios_print("NOT IMPLEMENTED: " #fn "\n");}

#define DEBUG(var) {bios_print(#var "= ");bios_print(var);bios_print("\n");}

#define _is_num(c) ((c)-'0' <= 9)

void _reverse(char s[]);