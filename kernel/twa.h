#ifndef _TWA_H
#define _TWA_H

void twa_init_1(void* twa_start, void* twa_end);
void twa_destroy();

void* twa_alloc(size_t size);
void twa_free(size_t size);

#endif
