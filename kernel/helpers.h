#ifndef _HELPERS_H
#define _HELPERS_H

#include <stddef.h>
#include <stdbool.h>

typedef struct _stack_frame_t {
  struct _stack_frame_t* next;
  void* ret;
} stack_frame_t;

size_t _get_call_stack(size_t max_depth, void** return_addresses);

void *_memcpy(void *restrict dst, const void *restrict src, size_t n);
void *_memset(void *b, int c, size_t len);

size_t _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
char *_strncpy(char * dst, const char * src, size_t len);
char *_strcpy(char * dst, const char * src);
char *_strrev(char *str);

void _format_unsigned(uint64_t number, bool base16, int width, char buffer[32]);
void _format_signed(int64_t number, bool base16, int width, char buffer[32]);

#endif
