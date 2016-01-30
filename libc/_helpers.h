#include "stdio.h"

#define NOT_IMPLEMENTED(fn)   {printf("NOT IMPLEMENTED: " #fn "\n");return 0;}
#define NOT_IMPLEMENTED_V(fn) {printf("NOT IMPLEMENTED: " #fn "\n");}

#define _is_num(c) (((c)-'0' <= 9) && ((c)-'0' >= 0))

void _reverse(char *s);
int _to_int(const char* begin, const char* end);
