#include <scr.h>

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

typedef struct _format_t {
  const char* format;
  size_t cursor;
  va_list* ap;
} format_t;

typedef struct _buffer_t {
  char* text;
  size_t length;
  size_t size;
} buffer_t;

typedef struct _specifier_t {
  int flag_left_justify;
  int flag_plus_sign;
  int flag_space_sign;
  int flag_base_or_decimal;
  int flag_zero_pad;
  int width;
  int precision;
  int modifier;
  int type;
} specifier_t;

typedef struct _text_t {
  const char* text;
  size_t length;
} text_t;

typedef struct _parsed_format_t {
  int type;
  union {
    text_t text;
    specifier_t specifier;
  };
} parsed_format_t;

/* Returns 0 if no more output possible, 1 otherwise */
typedef int (*write_fn_t)(void* out, char c);

FILE *stderr, *stdout;

static void format_init(format_t* fmt, const char* format, va_list *ap) {
  fmt->format = format;
  fmt->cursor = 0;
  fmt->ap = ap;
}

static void buffer_init(buffer_t* buffer, char* text, size_t size) {
  buffer->text = text;
  buffer->size = size;
  buffer->length = 0;
}

static void specifier_init(specifier_t* spec) {
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

static void text_init(text_t* text) {
  text->text = 0;
  text->length = 0;
}

static void parsed_format_init(parsed_format_t* pfmt) {
  pfmt->type = PF_EMPTY;
}

/* Returns 1 if text is fetched, 0 if [fmt] exhausted */
static int format_fetch_text(format_t* fmt, parsed_format_t* pfmt) {
  if (!fmt->format[fmt->cursor]) {
    return 0;
  }

  pfmt->type = PF_TEXT;
  pfmt->text.text = "";
  pfmt->text.length = 0;

  if ((fmt->format[fmt->cursor] == '%')
      && (fmt->format[fmt->cursor + 1] == '%')) {
    fmt->cursor += 2;
    pfmt->text.text = fmt->format + fmt->cursor - 1;
    pfmt->text.length = 1;
    return 1;
  }

  const char* begin = fmt->format + fmt->cursor;

  while ((fmt->format[fmt->cursor] != '%') && fmt->format[fmt->cursor]) {
    fmt->cursor++;
  }

  pfmt->text.text = begin;
  pfmt->text.length = fmt->format + fmt->cursor - begin;
  return 1;
}

/* Returns 1 if flags are fetched, 0 if [fmt] exhausted */
static int format_fetch_flags(format_t* fmt, parsed_format_t* pfmt) {
  if (!fmt->format[fmt->cursor]) {
    return 0;
  }

  pfmt->type = PF_SPECIFIER;
  pfmt->specifier.flag_left_justify = 0;
  pfmt->specifier.flag_plus_sign = 0;
  pfmt->specifier.flag_space_sign = 0;
  pfmt->specifier.flag_base_or_decimal = 0;
  pfmt->specifier.flag_zero_pad = 0;

  const char* begin = fmt->format + fmt->cursor;
  int valid = 1;

  while (fmt->format[fmt->cursor] && valid) {
    switch (fmt->format[fmt->cursor]) {
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
static int format_fetch_width(format_t* fmt, parsed_format_t* pfmt) {
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

  const char* begin = fmt->format + fmt->cursor;

  while (_is_num(fmt->format[fmt->cursor]) && fmt->format[fmt->cursor]) {
    fmt->cursor++;
  }

  pfmt->specifier.width = _to_int(begin, fmt->format + fmt->cursor);
  return 1;
}

/* Returns 1 if precision is fetched, 0 if [fmt] exhausted */
static int format_fetch_precision(format_t* fmt, parsed_format_t* pfmt) {
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

  const char* begin = fmt->format + fmt->cursor;

  while (_is_num(fmt->format[fmt->cursor]) && fmt->format[fmt->cursor]) {
    fmt->cursor++;
  }

  pfmt->specifier.precision = _to_int(begin, fmt->format + fmt->cursor);
  return 1;
}

/* Returns 1 if modifier is fetched, 0 if [fmt] exhausted */
static int format_fetch_modifier(format_t* fmt, parsed_format_t* pfmt) {
  if (!fmt->format[fmt->cursor]) {
    return 0;
  }

  pfmt->type = PF_SPECIFIER;
  pfmt->specifier.modifier = MOD_NONE;

  if ((fmt->format[fmt->cursor] == 'h')
      && (fmt->format[fmt->cursor + 1] == 'h')) {
    fmt->cursor += 2;
    pfmt->specifier.modifier = MOD_hh;
    return 1;
  }

  if ((fmt->format[fmt->cursor] == 'l')
      && (fmt->format[fmt->cursor + 1] == 'l')) {
    fmt->cursor += 2;
    pfmt->specifier.modifier = MOD_ll;
    return 1;
  }

  switch (fmt->format[fmt->cursor]) {
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
static int format_fetch_type(format_t* fmt, parsed_format_t* pfmt) {
  if (!fmt->format[fmt->cursor]) {
    return 0;
  }

  pfmt->type = PF_SPECIFIER;
  pfmt->specifier.type = TYPE_NONE;

  switch (fmt->format[fmt->cursor]) {
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

static int format_fetch_specifier(format_t* fmt, parsed_format_t* pfmt) {
  if (!fmt->format[fmt->cursor]) {
    return 0;
  }

  pfmt->type = PF_SPECIFIER;
  specifier_init(&pfmt->specifier);

  if (fmt->format[fmt->cursor] != '%') {
    return 1;
  }

  fmt->cursor++;

  int rc;

  rc = format_fetch_flags(fmt, pfmt);
  if (!rc) {
    return 0;
  }

  rc = format_fetch_width(fmt, pfmt);
  if (!rc) {
    return 0;
  }

  rc = format_fetch_precision(fmt, pfmt);
  if (!rc) {
    return 0;
  }

  rc = format_fetch_modifier(fmt, pfmt);
  if (!rc) {
    return 0;
  }

  return format_fetch_type(fmt, pfmt);
}

static int write_buffer(void* out, char c) {
  buffer_t* buffer = (buffer_t*) out;

  if (buffer->length < buffer->size) {
    buffer->text[buffer->length++] = c;
    return 1;
  }

  return 0;
}

static int write_scr(void *out, char c) {
  char sz[2] = { c, 0 };
  scr_print(sz);
  return 1;
}

static int write_file(void *out, char c) {
  return fputc(c, (FILE*) out);
}

static void specifier_process_signed(specifier_t* spec, format_t* fmt,
    write_fn_t write, void* out) {
  intmax_t val = 0;

  switch (spec->modifier) {
  case MOD_NONE:
    val = va_arg(*(fmt->ap), int);
    val &= 0xFFFFFFFF;
    break;
  case MOD_hh:
    val = va_arg(*(fmt->ap), int);
    val &= 0xFF;
    break;
  case MOD_h:
    val = va_arg(*(fmt->ap), int);
    val &= 0xFFFF;
    break;
  case MOD_l:
    val = va_arg(*(fmt->ap), long int);
    val &= 0xFFFFFFFF;
    break;
  case MOD_ll:
    val = va_arg(*(fmt->ap), long long int);
    break;
  case MOD_j:
    val = va_arg(*(fmt->ap), intmax_t);
    break;
  case MOD_z:
    val = va_arg(*(fmt->ap), size_t);
    val &= 0xFFFFFFFF;
    break;
  case MOD_t:
    val = va_arg(*(fmt->ap), ptrdiff_t);
    val &= 0xFFFFFFFF;
    break;
  }

  /* TODO: flags, width, precision */

  if (val == 0) {
    write(out, '0');
    return;
  }

  if (val < 0) {
    write(out, '-');
    val = -val;
  }

  int i = 0;
  char digits[32];
  do {
    digits[i++] = '0' + (val % 10);
    val /= 10;
  } while (val != 0);

  for (i--; i >= 0; i--) {
    if (!write(out, digits[i])) {
      return;
    }
  }
}

static void specifier_process_unsigned(specifier_t* spec, format_t* fmt,
    write_fn_t write, void* out, char* symbols) {

  uintmax_t val = 0;

  switch (spec->modifier) {
  case MOD_NONE:
    val = va_arg(*(fmt->ap), unsigned int);
    val &= 0xFFFFFFFF;
    break;
  case MOD_hh:
    val = va_arg(*(fmt->ap), unsigned int);
    val &= 0xFF;
    break;
  case MOD_h:
    val = va_arg(*(fmt->ap), unsigned int);
    val &= 0xFFFF;
    break;
  case MOD_l:
    val = va_arg(*(fmt->ap), unsigned long int);
    val &= 0xFFFFFFFF;
    break;
  case MOD_ll:
    val = va_arg(*(fmt->ap), unsigned long long int);
    break;
  case MOD_j:
    val = va_arg(*(fmt->ap), uintmax_t);
    break;
  case MOD_z:
    val = va_arg(*(fmt->ap), size_t);
    val &= 0xFFFFFFFF;
    break;
  case MOD_t:
    val = va_arg(*(fmt->ap), ptrdiff_t);
    val &= 0xFFFFFFFF;
    break;
  }

  /* TODO: flags, precision */

  /* Format number */
  int base = strlen(symbols), i = 0;
  char digits[32];
  do {
    digits[i++] = symbols[val % base];
    val /= base;
  } while (val != 0);

  /* Apply padding */
  if (spec->width > 0) {
    while (i < spec->width) {
      if (spec->flag_zero_pad) {
        digits[i++] = '0';
      } else {
        digits[i++] = ' ';
      }
    }
  }

  for (i--; i >= 0; i--) {
    if (!write(out, digits[i])) {
      return;
    }
  }
}

static void specifier_process_pointer(specifier_t* spec, format_t* fmt,
    write_fn_t write, void* out) {

  uint32_t val = va_arg(*(fmt->ap), uint32_t);

  const char* symbols = "0123456789ABCDEF";
  int base = 16, i = 0;
  char digits[16];
  do {
    digits[i++] = symbols[val % base];
    val /= base;
  } while (val != 0);

  while (i < 8) {
    digits[i++] = '0';
  }

  for (i--; i >= 0; i--) {
    if (!write(out, digits[i])) {
      return;
    }
  }
}

static void specifier_process_string(specifier_t* spec, format_t* fmt,
    write_fn_t write, void* out) {

  char* val = va_arg(*(fmt->ap), char*);

  while (*val) {
    if (!write(out, *(val++))) {
      return;
    }
  }
}

static void specifier_process_char(specifier_t* spec, format_t* fmt,
    write_fn_t write, void* out) {
  char val = (char) va_arg(*(fmt->ap), int);

  write(out, val);
}

static void parsed_format_process_text(parsed_format_t* pfmt, write_fn_t write,
    void* out) {
  if (pfmt->type == PF_TEXT) {
    for (int i = 0; i < pfmt->text.length; i++) {
      if (!write(out, pfmt->text.text[i])) {
        return;
      }
    }
  }
}

static void parsed_format_process_specifier(parsed_format_t* pfmt,
    format_t* fmt, write_fn_t write, void* out) {

  if (pfmt->type == PF_SPECIFIER) {
    switch (pfmt->specifier.type) {
    case TYPE_d:
      specifier_process_signed(&pfmt->specifier, fmt, write, out);
      break;

    case TYPE_u:
      specifier_process_unsigned(&pfmt->specifier, fmt, write, out,
          "0123456789");
      break;

    case TYPE_p:
      specifier_process_pointer(&pfmt->specifier, fmt, write, out);
      break;

    case TYPE_x:
      specifier_process_unsigned(&pfmt->specifier, fmt, write, out,
          "0123456789abcdef");
      break;

    case TYPE_X:
      specifier_process_unsigned(&pfmt->specifier, fmt, write, out,
          "0123456789ABCDEF");
      break;

    case TYPE_c:
      specifier_process_char(&pfmt->specifier, fmt, write, out);
      break;

    case TYPE_s:
      specifier_process_string(&pfmt->specifier, fmt, write, out);
      break;
    }
  }
}

static int format_process(format_t* fmt, write_fn_t write, void* out) {
  parsed_format_t pfmt;
  int written = 0;

  for (;;) {
    parsed_format_init(&pfmt);
    if (!format_fetch_text(fmt, &pfmt)) {
      return written;
    }

    parsed_format_process_text(&pfmt, write, out);

    parsed_format_init(&pfmt);
    if (!format_fetch_specifier(fmt, &pfmt)) {
      return written;
    }

    parsed_format_process_specifier(&pfmt, fmt, write, out);
  }

  // TODO: return written chars
  return written;
}

/******************************************************************************/

int fflush(FILE *stream)NOT_IMPLEMENTED(fflush)
int fputc(int c, FILE *stream)NOT_IMPLEMENTED(fput)
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems,
    FILE *restrict stream)NOT_IMPLEMENTED(fwrite)
FILE* fopen(const char *restrict filename, const char *restrict mode)NOT_IMPLEMENTED(
    fopen)
int fseek(FILE *stream, long offset, int whence)NOT_IMPLEMENTED(fseek)
long ftell(FILE *stream)NOT_IMPLEMENTED(ftell)
size_t fread(void *restrict ptr, size_t size, size_t nitems,
    FILE *restrict stream)NOT_IMPLEMENTED(fread)
int fclose(FILE *stream)NOT_IMPLEMENTED(fclose)

int fprintf(FILE * restrict stream, const char * restrict format, ...) {
  va_list ap;
  va_start(ap, format);

  format_t fmt;
  format_init(&fmt, format, &ap);

  int ret = format_process(&fmt, write_file, stream);

  va_end(ap);
  return ret;
}

#ifndef _TEST_
int printf(const char * restrict format, ...) {
  va_list ap;
  va_start(ap, format);

  format_t fmt;
  format_init(&fmt, format, &ap);

  int ret = format_process(&fmt, write_scr, NULL);

  va_end(ap);
  return ret;
}
#endif

int sprintf(char * restrict str, const char * restrict format, ...) {
  va_list ap;
  va_start(ap, format);

  format_t fmt;
  format_init(&fmt, format, &ap);

  buffer_t buffer;
  buffer_init(&buffer, str, SIZE_MAX);

  int ret = format_process(&fmt, write_buffer, &buffer);

  va_end(ap);
  return ret;
}

int snprintf(char * restrict str, size_t size, const char * restrict format,
    ...) {
  va_list ap;
  va_start(ap, format);

  format_t fmt;
  format_init(&fmt, format, &ap);

  buffer_t buffer;
  buffer_init(&buffer, str, size);

  int ret = format_process(&fmt, write_buffer, &buffer);

  va_end(ap);
  return ret;
}

int vsnprintf(char * restrict str, size_t size, const char * restrict format,
    va_list ap)NOT_IMPLEMENTED(vsnprintf) /* DOES NOT WORK BECAUSE OF ap {
 format_t fmt;
 format_init(&fmt, format, &ap);

 buffer_t buffer;
 buffer_init(&buffer, str, size);

 return format_process(&fmt, write_buffer, &buffer);
 }*/

int sscanf(const char *restrict s, const char *restrict format, ...)NOT_IMPLEMENTED(
    sscanf)

