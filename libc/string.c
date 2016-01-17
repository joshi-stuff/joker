#include "string.h"
#include "_helpers.h"

void *memset(void *b, int c, size_t len) NOT_IMPLEMENTED(memset)
void *memcpy(void *restrict dst, const void *restrict src, size_t n) NOT_IMPLEMENTED(memcpy)
void *memmove(void *dst, const void *src, size_t len) NOT_IMPLEMENTED(memmove)
int strncmp(const char *s1, const char *s2, size_t n) NOT_IMPLEMENTED(strncmp)

size_t strlen(const char *s) {
	size_t len = 0;
	while(*(s++)) {
		len++;
	}
	return len;
}

int memcmp(const void *s1, const void *s2, size_t n) NOT_IMPLEMENTED(memcmp)

int strcmp(const char *s1, const char *s2) {
	while(*(s1++) && *(s2++)) {
		if (*s1 != *s2) break;
	}
	return *s1 - *s2;
}

char *strcpy(char * dst, const char * src) {
	char* ret = dst;
	while(*src) {
		*(dst++) = *(src++);
	}
	return ret;
}
