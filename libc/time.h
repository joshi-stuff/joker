struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};
typedef unsigned int time_t;

struct tm *gmtime(const time_t *clock);
time_t time(time_t *tloc);
struct tm *localtime(const time_t *clock);
time_t mktime(struct tm *timeptr);
double difftime(time_t time1, time_t time0);
