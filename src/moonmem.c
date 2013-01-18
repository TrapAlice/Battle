#include "moonmem.h"
#undef malloc
#undef calloc
#undef free
#include <stdlib.h>
#include <stdio.h>

static moonmem* MOONMEM;

inline int findEmptySlot(size_t pSize);
inline memnode* findEmptyNode();

void MOONMEM_init(unsigned int pSize){
    MOONMEM = calloc(1,sizeof(moonmem)+pSize/4+pSize);
    MOONMEM->size = pSize;
    MOONMEM->head = (memnode*)calloc(sizeof(memnode)*pSize/2, sizeof(memnode));
    MOONMEM->memoryslots = (byte*)(MOONMEM+1);
    MOONMEM->memory = (byte*)(MOONMEM+1)+pSize/4;
    
}

void* MOONMEM_alloc(size_t pSize){
    memnode *data = findEmptyNode();
    data->size = pSize;
    int pos = findEmptySlot(pSize/4);
    data->memory = MOONMEM->memory+pos*4;
    data->pos = pos;
    return data->memory;
}

inline int findEmptySlot(size_t pSize){
    int x;
    byte* slots = calloc(pSize,sizeof(byte*));
    for( x = 0; x<(MOONMEM->size/4 - pSize); x++){
        int n = memcmp(MOONMEM->memoryslots+x, slots, pSize);
        if( n==0 ){
            memset(MOONMEM->memoryslots+x, x+1, pSize);
            return x;
        }
    }
    printf("Out of memory\n");
    MOONMEM_memdump();
    MOONMEM_uninit();
    exit(0);
}

inline memnode* findEmptyNode(){
    int x = 0;
    while((MOONMEM->head+x)->memory != NULL){
        x++;
        if (x > MOONMEM->size/2){
            printf("Out of empty nodes\n");
            MOONMEM_uninit();
            exit(0);
        }
    }
    return MOONMEM->head+x;
}

void MOONMEM_dealloc(void* ptr){
    int x = 0;
    while( (MOONMEM->head+x)->memory != ptr ){
        x++;
        if( x > MOONMEM->size/2){
            return;
        }
    }
    memnode* obj = MOONMEM->head+x;
    memset(obj->memory, 0, obj->size);
    obj->memory = NULL;
    memset(MOONMEM->memoryslots+obj->pos, 0, obj->size/4);

}

void MOONMEM_memout(){
    int x = 16;
    while( x < (sizeof(moonmem)+MOONMEM->size+MOONMEM->size/4) ){
        printf("%02X",*((byte*)(MOONMEM)+x));
        x++;
        if(x%4==0)printf(" ");
    }
    printf("\n\n");
}

void MOONMEM_memdump(){
    MOONMEM_memout();
    exit(0);
}

void MOONMEM_uninit(){
    MOONMEM_memout();
    free(MOONMEM);
}
