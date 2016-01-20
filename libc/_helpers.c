#include "string.h"

void _reverse(char s[]) {
	int length = strlen(s);
	int c, i, j;

	for (i = 0, j = length - 1; i < j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int _to_int(char* begin, char* end) {
	int i = 0, mag = 1;
	
	for(end--; end>=begin; end--) {
		i += (*end - '0') * mag;
		mag *= 10;
	}
	
	return i;
}