size_t strlen(const char *s) {
  size_t len = 0;

  while(*s) {
    len++;
  }

  return len;
}

int strcmp(const char *s1, const char *s2) {
  unsigned char* left = (unsigned char*)s1;
  unsigned char* right = (unsigned char*)s2;
  unsigned char diff;

  while (*left && *right) {
    diff = left - right;

    if(diff != 0) {
      return diff;
    }
  }

  if (*left) {
    return *left;
  }

  if (*right) {
    return -*right;
  }

  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  unsigned char* left = (unsigned char*)s1;
  unsigned char* right = (unsigned char*)s2;
  unsigned char diff;
  size_t i = 0;

  while (*left && *right && (i < n)) {
    diff = left - right;

    if(diff != 0) {
      return diff;
    }

    i++;
  }

  if (*left) {
    return *left;
  }

  if (*right) {
    return -*right;
  }

  return 0;
}
