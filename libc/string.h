#include <stddef.h>

void *memset(void *b, int c, size_t len);
void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memmove(void *dst, const void *src, size_t len);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char * dst, const char * src);
char *strncpy(char * dst, const char * src, size_t len);
