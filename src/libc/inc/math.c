typedef unsigned int u_int32_t;

typedef union
{
  double value;
  struct
  {
    u_int32_t msw;
    u_int32_t lsw;
  } parts;
} ieee_double_shape_type;

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

static const double huge = 1.0e300;

double floor(double x) {
  int32_t i0,i1,_j0;
	u_int32_t i,j;
	EXTRACT_WORDS(i0,i1,x);
	_j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(_j0<20) {
	    if(_j0<0) { 	/* raise inexact if x != 0 */
		if(huge+x>0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0>=0) {i0=i1=0;}
		    else if(((i0&0x7fffffff)|i1)!=0)
			{ i0=0xbff00000;i1=0;}
		}
	    } else {
		i = (0x000fffff)>>_j0;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(huge+x>0.0) {	/* raise inexact flag */
		    if(i0<0) i0 += (0x00100000)>>_j0;
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (_j0>51) {
	    if(_j0==0x400) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((u_int32_t)(0xffffffff))>>(_j0-20);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(huge+x>0.0) { 		/* raise inexact flag */
		if(i0<0) {
		    if(_j0==20) i0+=1;
		    else {
			j = i1+(1<<(52-_j0));
			if(j<i1) i0 +=1 ; 	/* got a carry */
			i1=j;
		    }
		}
		i1 &= (~i);
	    }
	}
	INSERT_WORDS(x,i0,i1);
	return x;
}

double ceil(double x);
double fmod(double x, double y);
double fmin(double x, double y);
double fmax(double x, double y);
double fabs(double x);
double pow(double x, double y);
double acos(double x);
double asin(double x);
double atan(double x);
double cos(double x);
double sin(double x);
double exp(double x);
double log(double x);
double sqrt(double x);
double tan(double x);
double atan2(double y, double x);
