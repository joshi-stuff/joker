#include "inttypes.h"
#include "stdint.h"
#include "_helpers.h"


/* time.h ********************************************************************/
#include "time.h"
struct tm *gmtime(const time_t *clock) NOT_IMPLEMENTED(gmtime)
time_t time(time_t *tloc) NOT_IMPLEMENTED(time)
struct tm *localtime(const time_t *clock) NOT_IMPLEMENTED(localtime)
time_t mktime(struct tm *timeptr) NOT_IMPLEMENTED(mktime)
double difftime(time_t time1, time_t time0) NOT_IMPLEMENTED(difftime)
