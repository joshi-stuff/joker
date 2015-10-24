#include "libc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int gettimeofday(struct timeval *restrict tp, void *restrict tzp) {
  return 0;
}

struct tm *gmtime_r(const time_t *clock, struct tm *result) {
  return NULL;
}

struct tm *localtime_r(const time_t *clock, struct tm *result) {
  return NULL;
}

time_t mktime(struct tm *timeptr) {
  return 0;
}

double difftime(time_t time1, time_t time0) {
  return 0;
}

char *strptime(const char *restrict buf, const char *restrict format, struct tm *restrict tm) {
  return NULL;
}

size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr) {
  return 0;
}
