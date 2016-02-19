#include "math.h"
#include "stdint.h"
#include "_helpers.h"

double fmod(double x, double y)NOT_IMPLEMENTED(fmod)

double fabs(double x) {
  return (x < 0) ? -x : x;
}

double floor(double x) {
  // TODO: check this hack
  return (double)(int64_t)x;
}

double atan2(double y, double x)NOT_IMPLEMENTED(atan2)
double tan(double x)NOT_IMPLEMENTED(tan)
double sqrt(double x)NOT_IMPLEMENTED(sqrt)
double sin(double x)NOT_IMPLEMENTED(sin)
double log(double x)NOT_IMPLEMENTED(log)
double exp(double x)NOT_IMPLEMENTED(exp)
double cos(double x)NOT_IMPLEMENTED(cos)
double ceil(double x)NOT_IMPLEMENTED(ceil)
double atan(double x)NOT_IMPLEMENTED(atan)
double asin(double x)NOT_IMPLEMENTED(asin)
double acos(double x)NOT_IMPLEMENTED(acos)
double pow(double x, double y)NOT_IMPLEMENTED(pow)
double fmax(double x, double y)NOT_IMPLEMENTED(fmax)
double fmin(double x, double y)NOT_IMPLEMENTED(fmin)
