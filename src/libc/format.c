#include "libc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int sprintf(char *__restrict buf, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vsnprintf(buf, SIZE_MAX, format, arg);
	va_end(arg);

	return rv;
}

int snprintf(char *__restrict buf, size_t size,
			 const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vsnprintf(buf, size, format, arg);
	va_end(arg);
	return rv;
}

int sscanf(const char * __restrict str, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vsscanf(str, format, arg);
	va_end(arg);

	return rv;
}

int vsscanf(const char *restrict s, const char *restrict format, va_list arg) {
	/* TODO */
	return 0;
}

int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap) {
	/* TODO */
	return 0;
}
