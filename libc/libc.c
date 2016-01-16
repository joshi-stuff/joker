#define NOT_IMPLEMENTED {return 0;}
#define NOT_IMPLEMENTED_V {}


/* math.h ********************************************************************/

#include "math.h"

double fmod(double x, double y) NOT_IMPLEMENTED
double fabs(double x) NOT_IMPLEMENTED
double floor(double x) NOT_IMPLEMENTED
double atan2(double y, double x) NOT_IMPLEMENTED
double tan(double x) NOT_IMPLEMENTED
double sqrt(double x) NOT_IMPLEMENTED
double sin(double x) NOT_IMPLEMENTED
double log(double x) NOT_IMPLEMENTED
double exp(double x) NOT_IMPLEMENTED
double cos(double x) NOT_IMPLEMENTED
double ceil(double x) NOT_IMPLEMENTED
double atan(double x) NOT_IMPLEMENTED
double asin(double x) NOT_IMPLEMENTED
double acos(double x) NOT_IMPLEMENTED
double pow(double x, double y) NOT_IMPLEMENTED
double fmax(double x, double y) NOT_IMPLEMENTED
double fmin(double x, double y) NOT_IMPLEMENTED

	
/* setjmp.h ******************************************************************/

#include "setjmp.h"

int setjmp(jmp_buf env) NOT_IMPLEMENTED
void longjmp(jmp_buf env, int val) NOT_IMPLEMENTED_V

	
/* stdio.h *******************************************************************/

#include "stdio.h"

int fprintf(FILE * restrict stream, const char * restrict format, ...) NOT_IMPLEMENTED
int fflush(FILE *stream) NOT_IMPLEMENTED
int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap) NOT_IMPLEMENTED
int snprintf(char * restrict str, size_t size, const char * restrict format, ...) NOT_IMPLEMENTED
int fputc(int c, FILE *stream) NOT_IMPLEMENTED
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED
int sscanf(const char *restrict s, const char *restrict format, ...) NOT_IMPLEMENTED
FILE *fopen(const char *restrict filename, const char *restrict mode) NOT_IMPLEMENTED
int fseek(FILE *stream, long offset, int whence) NOT_IMPLEMENTED
long ftell(FILE *stream) NOT_IMPLEMENTED
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED
int	fclose(FILE *stream) NOT_IMPLEMENTED
int sprintf(char * restrict str, const char * restrict format, ...) NOT_IMPLEMENTED
	
FILE *stderr, *stdout;


/* stdlib.h ******************************************************************/

#include "stdlib.h"

void abort(void) NOT_IMPLEMENTED_V
void *malloc(size_t size) NOT_IMPLEMENTED
void *realloc(void *ptr, size_t size) NOT_IMPLEMENTED
void free(void *ptr) NOT_IMPLEMENTED_V

	
/* string.h ******************************************************************/

#include "string.h"

void *memset(void *b, int c, size_t len) NOT_IMPLEMENTED
void *memcpy(void *restrict dst, const void *restrict src, size_t n) NOT_IMPLEMENTED
void *memmove(void *dst, const void *src, size_t len) NOT_IMPLEMENTED
int strncmp(const char *s1, const char *s2, size_t n) NOT_IMPLEMENTED
size_t strlen(const char *s) NOT_IMPLEMENTED
int memcmp(const void *s1, const void *s2, size_t n) NOT_IMPLEMENTED
int strcmp(const char *s1, const char *s2) NOT_IMPLEMENTED

	
/* time.h ********************************************************************/

#include "time.h"

struct tm *gmtime(const time_t *clock) NOT_IMPLEMENTED
time_t time(time_t *tloc) NOT_IMPLEMENTED
struct tm *localtime(const time_t *clock) NOT_IMPLEMENTED
time_t mktime(struct tm *timeptr) NOT_IMPLEMENTED
double difftime(time_t time1, time_t time0) NOT_IMPLEMENTED
