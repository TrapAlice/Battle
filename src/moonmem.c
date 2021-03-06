#include "moonmem.h"
#undef malloc
#undef calloc
#undef free
#include <stdlib.h>
#include <stdio.h>
#include "dbg.h"
#include <assert.h>

typedef struct memnode{
	size_t  size;
	byte   *memory;
	int     pos;
}memnode;

typedef struct{
	unsigned int  size;
	memnode      *nodes;
	byte         *memoryslots;
	byte         *memory;
}moonmem;

moonmem* MOONMEM;

static int findEmptySlot(size_t pSize){
	int x;
	int n;
	byte* slots = calloc(pSize,sizeof(byte*));
	for( x = 0; x<(MOONMEM->size/4 - pSize); x++){
		n = memcmp(MOONMEM->memoryslots+x, slots, pSize);
		if( n==0 ){
			memset(MOONMEM->memoryslots+x, 0xFF, pSize);
			return x;
		}
	}
	log_err("Out of memory\n");
	memdump();
	uninitMoonMem();
	exit(0);
}

static memnode* findEmptyNode(){
	int x = 0;
	while((MOONMEM->nodes+x)->memory != NULL){
		x++;
		if (x > MOONMEM->size/2){
			log_err("Out of empty nodes\n");
			uninitMoonMem();
			exit(0);
		}
	}
	return MOONMEM->nodes+x;
}

void initMoonMem(unsigned int pSize){
	MOONMEM = calloc(1,sizeof(moonmem)+pSize/4+pSize);
	MOONMEM->size = pSize;
	MOONMEM->nodes = calloc(sizeof(memnode)*pSize/2, sizeof(memnode));
	MOONMEM->memoryslots = (byte*)(MOONMEM+1);
	MOONMEM->memory = (byte*)(MOONMEM+1)+pSize/4;
	
}

void* moonAlloc(size_t pSize){
	memnode *data = findEmptyNode();
	int pos;
	data->size = pSize;
	pos = findEmptySlot(pSize/4);
	data->memory = MOONMEM->memory+pos*4;
	data->pos = pos;
	return data->memory;
}



void moonDealloc(void* ptr){
	int x = 0;
	memnode* obj;
	if( !ptr ) {
		log_warn("Attempted to dealloc a null pointer");
		return;
	}
	while( (MOONMEM->nodes+x)->memory != ptr ){
		x++;
		if( x > MOONMEM->size/2){
			log_warn("Attempted to dealloc %p, not found",ptr);
			return;
		}
	}
	obj = MOONMEM->nodes+x;
	memset(obj->memory, 0, obj->size);
	obj->memory = NULL;
	memset(MOONMEM->memoryslots+obj->pos, 0, obj->size/4);


}

void memout(){
	int x = 16;
	while( x < (sizeof(moonmem)+MOONMEM->size+MOONMEM->size/4) ){
		printf("%08X ",*((int*)(MOONMEM)+x));
		x++;
	}
	printf("\n\n");
}

void objectMemout(void* ptr){
	int x = 0;
	memnode* obj;
	if(ptr==NULL)return;
	while( (MOONMEM->nodes+x)->memory != ptr ){
		x++;
		if( x > MOONMEM->size/2){
			return;
		}
	}
	obj = MOONMEM->nodes+x;

	printf("ptr   -> %p\n",ptr);
	printf("size  -> %d\n",obj->size);
	printf("memory-> ");
	x=0;
	while(x<obj->size){
		printf("%08X ",*((int*)(obj->memory)+x));
		x++;
	}
	printf("\n");
}

void memdump(){
	memout();
	exit(0);
}

void uninitMoonMem(){
	int x;
	int leek=0;
	memnode* node;
	for( x=0; x<MOONMEM->size/2; ++x ){
		node = MOONMEM->nodes+x;
		if( node->memory ){
			log_err("Leek: %p",node->memory);
			leek+=node->size;
			objectMemout(node->memory);
		}
	}
	if( leek ){
		log_err("Total memory leek: %d",leek);
	}
	free(MOONMEM->nodes);
	free(MOONMEM);
}
