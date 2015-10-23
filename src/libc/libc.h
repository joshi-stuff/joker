#ifndef POLYFILLS_H_INCLUDED
#define POLYFILLS_H_INCLUDED

#define SEEK_SET 0
#define SEEK_END 2

typedef struct _FILE {

} FILE;

typedef struct _jmp_buf {

} jmp_buf;

FILE* stdin;
FILE* stdout;
FILE* stderr;

void abort(void);

int sprintf(char * restrict str, const char * restrict format, ...);
int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap);
int snprintf(char * restrict str, size_t size, const char * restrict format, ...);

int sscanf(const char *restrict s, const char *restrict format, ...);

void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memset(void *b, int c, size_t len);
void *memmove(void *dst, const void *src, size_t len);
int memcmp(const void *s1, const void *s2, size_t n);

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

double floor(double x);
double fmod(double x, double y);
double fmin(double x, double y);
double fmax(double x, double y);
double fabs(double x);
double pow(double x, double y);
double acos(double x);
double asin(double x);
double atan(double x);
double ceil(double x);
double cos(double x);
double sin(double x);
double exp(double x);
double log(double x);
double sqrt(double x);
double tan(double x);
double atan2(double y, double x);

FILE *fopen(const char *restrict filename, const char *restrict mode);
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
int fputc(int c, FILE *stream);
int fprintf(FILE * restrict stream, const char * restrict format, ...);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int fflush(FILE *stream);
int fclose(FILE *stream);

void longjmp(jmp_buf env, int val);
int setjmp(jmp_buf env);

double _get_now();
int _get_tz_offset(double utcinstant);

#endif
