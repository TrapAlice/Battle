#include "tap.h"
#include "moonmem.h"
#include "libtcod.h"
#include <stdarg.h>
#include "test_ids.h"


/*int memory_test(){
	int* x;
	plan(NO_PLAN);
	MOONMEM_init(256);

	note("Testing Memory Manager's Memory Allocation and Deallocation");
	
	x = (int*)MOONMEM_alloc(sizeof(int*));
	*x = 0xBEEF;
	ok(0xBEEF == *x);
	MOONMEM_dealloc(x);
	ok(0x0 == *x);

    MOONMEM_uninit();
	done_testing();
}*/

void runTest(int id){
	switch(id){
		case MEMORY_TEST:
			//memory_test();
			break;
	}
}

void RunTests(int num, ...){
	int i;
	int val;
	va_list vl;
	va_start(vl,num);
	for( i = 0; i < num; i++){
		val = va_arg(vl,int);
		runTest(val);
	}
	va_end(vl);
}