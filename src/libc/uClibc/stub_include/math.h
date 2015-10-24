#ifndef _MATH_H_
#define _MATH_H_

#include "common.h"

#pragma GCC diagnostic ignored "-Wsign-compare"

#define _IEEE_ 0
#define _POSIX_ 1
#define _SVID_ 2
#define _XOPEN_ 3
#define _LIB_VERSION _POSIX_

# define HUGE		3.40282347e+38F

/* Types of exceptions in the `type' field.  */
# define DOMAIN		1
# define SING		2
# define OVERFLOW	3
# define UNDERFLOW	4
# define TLOSS		5
# define PLOSS		6

/* Return nonzero value if X is not +-Inf or NaN.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define isfinite(x) \
     (sizeof (x) == sizeof (float) ? __finitef (x) : __finite (x))
# else
#  define isfinite(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __finitef (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __finite (x) : __finitel (x))
# endif

/* Return nonzero value if X is a NaN.  We could use `fpclassify' but
   we already have this functions `__isnan' and it is faster.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define isnan(x) \
     (sizeof (x) == sizeof (float) ? __isnanf (x) : __isnan (x))
# else
#  define isnan(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __isnanf (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __isnan (x) : __isnanl (x))
# endif

typedef uint32_t u_int32_t;

int __finitef(float x);
int __finite(double x);
int __finitel(long double x);
int __isnan(double x);
int __isnanf(float x);
int __isnanl(long double x);
double __ieee754_pow(double x, double y);

#endif /* _MATH_H_ */
