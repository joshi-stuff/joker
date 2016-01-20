#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "_helpers.h"

#define WIDTH_FETCH     -1
#define PRECISION_NONE  -2
#define PRECISION_FETCH -1
#define MOD_NONE 		    0
#define MOD_hh   		    1
#define MOD_h    		    2
#define MOD_l    		    3
#define MOD_ll   		    4
#define MOD_j    		    5
#define MOD_z    		    6
#define MOD_t    		    7
#define MOD_L    		    8
#define TYPE_NONE 			-1
#define TYPE_d  				0
#define TYPE_u				  1
#define TYPE_o				  2
#define TYPE_x				  3
#define TYPE_X				  4
#define TYPE_f				  5
#define TYPE_F				  6
#define TYPE_e				  7
#define TYPE_E				  8
#define TYPE_g				  9
#define TYPE_G				  10
#define TYPE_a				  11
#define TYPE_A				  12
#define TYPE_c				  13
#define TYPE_s				  14
#define TYPE_p				  15
#define TYPE_n				  16
#define PF_EMPTY				0
#define PF_TEXT					1
#define PF_SPECIFIER		2

typedef struct __format {
	char* format;
	size_t cursor;
	va_list* ap;
} _format;

typedef struct __buffer {
	char* text;
	size_t length;
	size_t size;
} _buffer;

typedef struct __specifier {
	int flag_left_justify;
	int flag_plus_sign;
	int flag_space_sign;
	int flag_base_or_decimal;
	int flag_zero_pad;
	int width;
	int precision;
	int modifier;
	int type;
} _specifier;

typedef struct __text {
	char* text;
	size_t length;
} _text;

typedef struct __parsed_format {
	int type;
	union {
		_text text;	
		_specifier specifier;
	};
} _parsed_format;

/* Returns 0 if no more output possible, 1 otherwise */
typedef int (*_write_fn)(void* out, char c);

FILE *stderr, *stdout;

void _init_format(_format* fmt, char* format, va_list *ap) {
	fmt->format = format;
	fmt->cursor = 0;
	fmt->ap = ap;
}

void _init_buffer(_buffer* buffer, char* text, size_t size) {
	buffer->text = text;
	buffer->size = size;
	buffer->length = 0;
}

void _init_specifier(_specifier* spec) {
	spec->flag_left_justify = 0;
	spec->flag_plus_sign = 0;
	spec->flag_space_sign = 0;
	spec->flag_base_or_decimal = 0;
	spec->flag_zero_pad = 0;
	spec->width = 0;
	spec->precision = 0;
	spec->modifier = MOD_NONE;
	spec->type = TYPE_NONE;
}

void _init_text(_text* text) {
	text->text = 0;
	text->length = 0;
}

void _init_parsed_format(_parsed_format* pfmt) {
	pfmt->type = PF_EMPTY;
}

/* Returns 1 if text is fetched, 0 if [fmt] exhausted */
int _fetch_text(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_TEXT;
	pfmt->text.text = "";
	pfmt->text.length = 0;

	if ((fmt->format[fmt->cursor] == '%') && (fmt->format[fmt->cursor+1] == '%')) {
		fmt->cursor += 2;
		pfmt->text.text = fmt->format + fmt->cursor - 1;
		pfmt->text.length = 1;
		return 1;
	}

	char* begin = fmt->format + fmt->cursor;
	
	while((fmt->format[fmt->cursor] != '%') && fmt->format[fmt->cursor]) {
		fmt->cursor++;
	}
	
	pfmt->text.text = begin;
	pfmt->text.length = fmt->format + fmt->cursor - begin;	
	return 1;
}

/* Returns 1 if flags are fetched, 0 if [fmt] exhausted */
int _fetch_flags(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	pfmt->specifier.flag_left_justify = 0;
	pfmt->specifier.flag_plus_sign = 0;
	pfmt->specifier.flag_space_sign = 0;
	pfmt->specifier.flag_base_or_decimal = 0;
	pfmt->specifier.flag_zero_pad = 0;
	
	char* begin = fmt->format + fmt->cursor;
	int valid = 1;

	while(fmt->format[fmt->cursor] && valid) {
		switch(fmt->format[fmt->cursor]) {
			case '-':
				pfmt->specifier.flag_left_justify = 1;
				fmt->cursor++;
				break;
				
			case '+':
				pfmt->specifier.flag_plus_sign = 1;
				fmt->cursor++;
				break;
				
			case ' ':
				pfmt->specifier.flag_space_sign = 1;
				fmt->cursor++;
				break;

			case '#':
				pfmt->specifier.flag_base_or_decimal = 1;
				fmt->cursor++;
				break;

			case '0':
				pfmt->specifier.flag_zero_pad = 1;
				fmt->cursor++;
				break;
				
			default: 
				valid = 0;
				break;
		}		
	}

	return 1;
}

/* Returns 1 if width is fetched, 0 if [fmt] exhausted */
int _fetch_width(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	pfmt->specifier.width = 0;
	
	if (fmt->format[fmt->cursor] == '*') {
		fmt->cursor += 1;
		pfmt->specifier.width = WIDTH_FETCH;
		return 1;
	}

	char* begin = fmt->format + fmt->cursor;
	
	while(_is_num(fmt->format[fmt->cursor]) && fmt->format[fmt->cursor]) {
		fmt->cursor++;
	}
	
	pfmt->specifier.width = _to_int(begin, fmt->format + fmt->cursor);
	return 1;
}

/* Returns 1 if precision is fetched, 0 if [fmt] exhausted */
int _fetch_precision(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	pfmt->specifier.precision = PRECISION_NONE;
	
	if (fmt->format[fmt->cursor] != '.') {
		return 1;
	}
	
	fmt->cursor++;
	
	if (fmt->format[fmt->cursor] == '*') {
		fmt->cursor += 1;
		pfmt->specifier.precision = PRECISION_FETCH;
		return 1;
	}

	char* begin = fmt->format + fmt->cursor;
	
	while(_is_num(fmt->format[fmt->cursor]) && fmt->format[fmt->cursor]) {
		fmt->cursor++;
	}
	
	pfmt->specifier.precision = _to_int(begin, fmt->format + fmt->cursor);
	return 1;
}

/* Returns 1 if modifier is fetched, 0 if [fmt] exhausted */
int _fetch_modifier(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	pfmt->specifier.modifier = MOD_NONE;
	
	if ((fmt->format[fmt->cursor] == 'h') && (fmt->format[fmt->cursor+1] == 'h')) {
		fmt->cursor += 2;
		pfmt->specifier.modifier = MOD_hh;
		return 1;
	}
	
	if ((fmt->format[fmt->cursor] == 'l') && (fmt->format[fmt->cursor+1] == 'l')) {
		fmt->cursor += 2;
		pfmt->specifier.modifier = MOD_ll;
		return 1;
	}

	switch(fmt->format[fmt->cursor]) {
		case 'h':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_h;
			break;			
			
		case 'l':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_l;
			break;			
			
		case 'j':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_j;
			break;			
			
		case 'z':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_z;
			break;			
			
		case 't':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_t;
			break;			
			
		case 'L':
			fmt->cursor += 1;
			pfmt->specifier.modifier = MOD_L;
			break;			
	}
	
	return 1;
}

/* Returns 1 if specifier is fetched, 0 if [fmt] exhausted */
int _fetch_type(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	pfmt->specifier.type = TYPE_NONE;
	
	switch(fmt->format[fmt->cursor]) {
		case 'd':
		case 'i':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_d;
			break;			

		case 'u':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_u;
			break;			

		case 'o':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_o;
			break;			

		case 'x':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_x;
			break;			

		case 'X':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_X;
			break;			

		case 'f':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_f;
			break;			

		case 'F':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_F;
			break;			

		case 'e':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_e;
			break;			

		case 'E':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_E;
			break;			

		case 'g':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_g;
			break;			

		case 'G':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_G;
			break;			

		case 'a':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_a;
			break;			

		case 'A':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_A;
			break;			

		case 'c':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_c;
			break;			

		case 's':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_s;
			break;			

		case 'p':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_p;
			break;			

		case 'n':
			fmt->cursor += 1;
			pfmt->specifier.type = TYPE_n;
			break;			
	}
	
	return 1;
}

int _fetch_specifier(_format* fmt, _parsed_format* pfmt) {
	if (!fmt->format[fmt->cursor]) {
		return 0;
	}

	pfmt->type = PF_SPECIFIER;
	_init_specifier(&pfmt->specifier);
		
	if(fmt->format[fmt->cursor] != '%') {
		return 1;
	}
	
	fmt->cursor++;
	
	int rc;
	
	rc = _fetch_flags(fmt, pfmt);
	if (!rc) {
		return 0;
	}
	
	rc = _fetch_width(fmt, pfmt);
	if (!rc) {
		return 0;
	}
	
	rc = _fetch_precision(fmt, pfmt);
	if (!rc) {
		return 0;
	}
	
	rc = _fetch_modifier(fmt, pfmt);
	if (!rc) {
		return 0;
	}
	
	return _fetch_type(fmt, pfmt);
}

int _write_buffer(void* out, char c) {
	_buffer* buffer = (_buffer*)out;
	
	if(buffer->length < buffer->size) {
		buffer->text[buffer->length++] = c;
		return 1;
	}
	
	return 0;
}

void _write_bios(void *out, char c) {
	char sz[2] = {c, 0};
	bios_print(sz);
}

void _process_text(_parsed_format* pfmt, _write_fn write, void* out) {
	if(pfmt->type == PF_TEXT) {
		for(int i=0; i<pfmt->text.length; i++) {
			if(!write(out, pfmt->text.text[i])) {
				return;
			}
		}
	}
}

void _process_specifier(_parsed_format* pfmt, _write_fn write, void* out) {
	if(pfmt->type == PF_SPECIFIER) {

	}
}

void _process_format(_format* fmt, _write_fn write, void* out) {
	_parsed_format pfmt;

	for( ; ; ) {
		_init_parsed_format(&pfmt);
		if(!_fetch_text(fmt, &pfmt)) {
			return;
		}
		
		_process_text(&pfmt, write, out);

		_init_parsed_format(&pfmt);
		if(!_fetch_specifier(fmt, &pfmt)) {
			return;
		}
		
		_process_specifier(&pfmt, write, out);
	}
}


/* **************************************************************************************** */

int fprintf(FILE * restrict stream, const char * restrict format, ...) NOT_IMPLEMENTED(fprintf)
int fflush(FILE *stream) NOT_IMPLEMENTED(fflush)
int fputc(int c, FILE *stream) NOT_IMPLEMENTED(fput)
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED(fwrite)
FILE* fopen(const char *restrict filename, const char *restrict mode) NOT_IMPLEMENTED(fopen)
int fseek(FILE *stream, long offset, int whence) NOT_IMPLEMENTED(fseek)
long ftell(FILE *stream) NOT_IMPLEMENTED(ftell)
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) NOT_IMPLEMENTED(fread)
int	fclose(FILE *stream) NOT_IMPLEMENTED(fclose)

int sscanf(const char *restrict s, const char *restrict format, ...) NOT_IMPLEMENTED(sscanf)
	
/*
int printf(const char * restrict format, ...) {
	va_list ap;
	va_start(ap, format);
	// TODO: do not rely on a buffer
	char str[8192];
	int ret = vsnprintf(str, sizeof(str), format, ap);
	bios_print(str);
	va_end(ap);
	return ret;
}
*/

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

int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap) {
	char* start = str;
	
		
	return str - start;
}