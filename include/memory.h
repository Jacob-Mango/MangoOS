#ifndef __MEMORY__H__
#define __MEMORY__H__

#include <common/types.h>

void memcpy(char *from, char *to, int count);
void memcpyTF(char *to, char *from, int count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
void *memset(void *dest, char val, size_t count);
bool memzero(void *dest, size_t len);
void *malloc(int bytes);
void *malloc_p(int bytes);
void *malloc_ps(char *start, int bytes);
bool pm_setup(char *heap);

#endif