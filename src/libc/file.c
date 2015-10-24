#include "libc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static FILE _stdin;
static FILE _stdout;
static FILE _stderr;

FILE* stdin = &_stdin;
FILE* stdout = &_stdout;
FILE* stderr = &_stderr;

FILE *fopen(const char *restrict filename, const char *restrict mode) {
  return NULL;
}

size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) {
  return 0;
}

size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) {
  return 0;
}

int fputs(const char *restrict s, FILE *restrict stream) {
  return 0;
}

int fputc(int c, FILE *stream) {
  return 0;
}

int fprintf(FILE * restrict stream, const char * restrict format, ...) {
  return 0;
}

int fseek(FILE *stream, long offset, int whence) {
  return 0;
}

long ftell(FILE *stream) {
  return 0;
}

int fflush(FILE *stream) {
  return 0;
}

int fclose(FILE *stream) {
  return 0;
}
