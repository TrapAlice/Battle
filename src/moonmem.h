#ifndef _MOONMEM_H
#define _MOONMEM_H

#include <string.h>

typedef unsigned char byte;

#define malloc(x) MOONMEM_alloc(x)
#define calloc(x,y) MOONMEM_alloc(x*y)
#define free(x) MOONMEM_dealloc(x)


typedef struct memnode{
    size_t size;
    byte* memory;
    int pos;
}memnode;

typedef struct{
	unsigned int size;
	memnode* head;
	byte* memoryslots;
    byte* memory;
}moonmem;



void MOONMEM_init(unsigned int pSize);
void* MOONMEM_alloc(size_t pSize);
void MOONMEM_dealloc(void* ptr);
void MOONMEM_memout();
void MOONMEM_memdump();
void MOONMEM_uninit();

#endif
