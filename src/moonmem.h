#ifndef _MOONMEM_H
#define _MOONMEM_H

#include <string.h>

typedef unsigned char byte;

#define malloc(x) moonAlloc(x)
#define calloc(x,y) moonAlloc(x*y)
#define free(x) moonDealloc(x)

void initMoonMem(unsigned int pSize);
void* moonAlloc(size_t pSize);
void moonDealloc(void* ptr);
void memout();
void objectMemout(void* ptr);
void memdump();
void uninitMoonMem();

#endif
