#ifndef LIBC_H_INCLUDED
#define LIBC_H_INCLUDED

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

/* control */
# define EDOM	33
# define ERANGE	34

typedef struct _jmp_buf {
} jmp_buf;

extern int errno;

void abort(void);
void longjmp(jmp_buf env, int val);
int setjmp(jmp_buf env);

/* format */
int sprintf(char * restrict str, const char * restrict format, ...);
int snprintf(char * restrict str, size_t size, const char * restrict format, ...);
int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap);
int sscanf(const char *restrict s, const char *restrict format, ...);
int vsscanf(const char *restrict s, const char *restrict format, va_list arg);

/* memory */
void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memset(void *b, int c, size_t len);
void *memmove(void *dst, const void *src, size_t len);
int memcmp(const void *s1, const void *s2, size_t n);

/* alloc */
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

/* string */
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

/* math */
struct exception {
  int type;
  char *name;
  double arg1;
  double arg2;
  double retval;
};

int isnan(double x);
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
double scalbn(double x, int n);
double copysign(double x, double y);
double rint(double x);
int matherr(struct exception *x);

/* file */
#define SEEK_SET 0
#define SEEK_END 2

typedef struct _FILE {
} FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

FILE *fopen(const char *restrict filename, const char *restrict mode);
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
int fputs(const char *restrict s, FILE *restrict stream);
int fputc(int c, FILE *stream);
int fprintf(FILE * restrict stream, const char * restrict format, ...);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int fflush(FILE *stream);
int fclose(FILE *stream);

/* time */
typedef int32_t time_t;
typedef int32_t suseconds_t;

struct tm {
        int     tm_sec;         /* seconds after the minute [0-60] */
        int     tm_min;         /* minutes after the hour [0-59] */
        int     tm_hour;        /* hours since midnight [0-23] */
        int     tm_mday;        /* day of the month [1-31] */
        int     tm_mon;         /* months since January [0-11] */
        int     tm_year;        /* years since 1900 */
        int     tm_wday;        /* days since Sunday [0-6] */
        int     tm_yday;        /* days since January 1 [0-365] */
        int     tm_isdst;       /* Daylight Savings Time flag */
        long    tm_gmtoff;      /* offset from CUT in seconds */
        char    *tm_zone;       /* timezone abbreviation */
};

struct timeval {
  time_t       tv_sec;   /* seconds since Jan. 1, 1970 */
  suseconds_t  tv_usec;  /* and microseconds */
};

struct timezone {
  int     tz_minuteswest; /* of Greenwich */
  int     tz_dsttime;     /* type of dst correction to apply */
};

int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
struct tm *gmtime_r(const time_t *clock, struct tm *result);
struct tm *localtime_r(const time_t *clock, struct tm *result);
time_t mktime(struct tm *timeptr);
double difftime(time_t time1, time_t time0);
char *strptime(const char *restrict buf, const char *restrict format, struct tm *restrict tm);
size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr);

#endif /* LIBC_H_INCLUDED */
