FILE *fopen(const char *restrict filename, const char *restrict mode);
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
int fputc(int c, FILE *stream);
int fprintf(FILE * restrict stream, const char * restrict format, ...);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int fflush(FILE *stream);
int fclose(FILE *stream);
