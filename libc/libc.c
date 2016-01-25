#include "inttypes.h"
#include "stdint.h"
#include "_helpers.h"


/* math.h ********************************************************************/
#include "math.h"
double fmod(double x, double y) NOT_IMPLEMENTED(fmod)
double fabs(double x) NOT_IMPLEMENTED(fabs)
double floor(double x) NOT_IMPLEMENTED(floor)
double atan2(double y, double x) NOT_IMPLEMENTED(atan2)
double tan(double x) NOT_IMPLEMENTED(tan)
double sqrt(double x) NOT_IMPLEMENTED(sqrt)
double sin(double x) NOT_IMPLEMENTED(sin)
double log(double x) NOT_IMPLEMENTED(log)
double exp(double x) NOT_IMPLEMENTED(exp)
double cos(double x) NOT_IMPLEMENTED(cos)
double ceil(double x) NOT_IMPLEMENTED(ceil)
double atan(double x) NOT_IMPLEMENTED(atan)
double asin(double x) NOT_IMPLEMENTED(asin)
double acos(double x) NOT_IMPLEMENTED(acos)
double pow(double x, double y) NOT_IMPLEMENTED(pow)
double fmax(double x, double y) NOT_IMPLEMENTED(fmax)
double fmin(double x, double y) NOT_IMPLEMENTED(fmin)

	
/* setjmp.h ******************************************************************/
#include "setjmp.h"
int setjmp(jmp_buf env) NOT_IMPLEMENTED(setjmp)
void longjmp(jmp_buf env, int val) NOT_IMPLEMENTED_V(longjmp)

	
/* time.h ********************************************************************/
#include "time.h"
struct tm *gmtime(const time_t *clock) NOT_IMPLEMENTED(gmtime)
time_t time(time_t *tloc) NOT_IMPLEMENTED(time)
struct tm *localtime(const time_t *clock) NOT_IMPLEMENTED(localtime)
time_t mktime(struct tm *timeptr) NOT_IMPLEMENTED(mktime)
double difftime(time_t time1, time_t time0) NOT_IMPLEMENTED(difftime)
