typedef struct {} jmp_buf;

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);