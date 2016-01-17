#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "_helpers.h"

FILE *stderr, *stdout;

int fprintf(FILE * restrict stream, const char * restrict format, ...) NOT_IMPLEMENTED(fprintf)
int fflush(FILE *stream) NOT_IMPLEMENTED(fflush)
int fputc(int c, FILE *stream) NOT_IMPLEMENTED(fput)
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED(fwrite)
FILE *fopen(const char *restrict filename, const char *restrict mode) NOT_IMPLEMENTED(fopen)
int fseek(FILE *stream, long offset, int whence) NOT_IMPLEMENTED(fseek)
long ftell(FILE *stream) NOT_IMPLEMENTED(ftell)
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED(fread)
int	fclose(FILE *stream) NOT_IMPLEMENTED(fclose)

int sscanf(const char *restrict s, const char *restrict format, ...) NOT_IMPLEMENTED(sscanf)
	
int sprintf(char * restrict str, const char * restrict format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = vsnprintf(str, UINT32_MAX, format, ap);
	va_end(ap);
	return ret;
}
	
int snprintf(char * restrict str, size_t size, const char * restrict format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = vsnprintf(str, size, format, ap);
	va_end(ap);
	return ret;
}

int _parse_specifier(const char* format, char *flags, int *width, char* type, va_list* ap) {
	char specifier[256];
	int i = 0, j = 0, finished = 0;

	specifier[j++] = format[i++];

	while(!finished) {
		switch(format[i]) {
			case 'd':
			case 'i':
			case 'u':
			case 'o':
			case 'x':
			case 'X':
			case 'f':
			case 'F':
			case 'e':
			case 'E':
			case 'g':
			case 'G':
			case 'a':
			case 'A':
			case 'c':
			case 's':
			case 'p':
			case 'n':
			case '%':
				specifier[j++] = format[i++];
				finished = 1;
				break;

			default:
				specifier[j++] = format[i++];
				break;
		}
	}

	specifier[j] = 0;

	/* Digest whole specifier */
	int k = 1;

	/* Flags field */
	switch(specifier[k]) {
		case '-':
		case '+':
		case ' ':
		case '#':
		case '0':
			*flags = specifier[k++];
			break;
	}

	/* Width field */
	if (specifier[k] == '*') {
		*width = va_arg(*ap, int);
		k++;
	} else if(_is_num(specifier[k])) {
		char wstr[128];
		int l = 0;

		while(_is_num(specifier[k])) {
			wstr[l++] = specifier[k++];
		}
		wstr[l] = 0;

		*width = atoi(wstr);
	}

	/* TODO: Precision field */

	/* TODO: Modifier field */

	/* Type field */
	int l = 0;
	for(; specifier[k]; l++, k++) {
		type[l] = specifier[k];
	}
	type[l] = 0;	
	
	return i;
}

char* _interpret_specifier(char * restrict str, char flags, int width, char* type, va_list* ap) {
	if(!strcmp(type, "X")) {
		unsigned val = va_arg(*ap, unsigned);
		char output[128];

		if(val==0) {
			strcpy(output, "0");
		} else {
			int m = 0;

			while(val) {
				unsigned digit = val % 16;
				if(digit > 9) {
					output[m++] = 'A' + (digit-10);
				} else {
					output[m++] = '0' + digit;
				}
				val >>= 4;
			}
			output[m] = 0;
		}

		// TODO: honor flags

		_reverse(output);		

		strcpy(str, output);
		str += strlen(output);
	} else {
		*(str++) = '[';
		strcpy(str, type);
		str += strlen(type);
		*(str++) = ']';
	}
	
	return str;
}

int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap) {
	char* start = str;
	
	for (size_t i = 0; i < size && format[i]; ) {
		if(format[i] == '%') {
			char flags;
			int width;
			char type[128];
			
			i += _parse_specifier(format+i, &flags, &width, type, &ap);
			str = _interpret_specifier(str, flags, width, type, &ap);
		} else {
			*(str++) = format[i++];
		}
	}
	*(str++) = 0;
		
	return str - start;
}

