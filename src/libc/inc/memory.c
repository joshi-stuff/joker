void *memcpy(void *restrict dst, const void *restrict src, size_t n) {
  char* source = (char*)src;
  char* dest = (char*)dst;

  for (size_t i=0; i<n; i++) {
    dest[i] = source[i];
  }

  return dst;
}

void *memset(void *b, int c, size_t len) {
  char* dest = (char*)b;

  for (size_t i=0; i<len; i++) {
    dest[i] = c;
  }

  return b;
}

void *memmove(void *dst, const void *src, size_t len) {
  char* source = (char*)src;
  char* dest = (char*)dst;

  if(src < dst) {
    for (size_t i=len, j=0; j<len; i--,j++) {
      dest[i] = source[i];
    }
  }
  else if(dst < src) {
    for (size_t i=0; i<len; i++) {
      dest[i] = source[i];
    }
  }

  return dst;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char* left = (unsigned char*)s1;
  unsigned char* right = (unsigned char*)s2;
  unsigned char diff;

  for (size_t i=0; i<n; i++) {
    diff = left[i] - right[i];

    if(diff != 0) {
      return diff;
    }
  }

  return 0;
}
