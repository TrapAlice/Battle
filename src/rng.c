#include "rng.h"
#include <time.h>
#include <stdlib.h>

void initSeed(int seed){
	srand(seed==0 ? time(NULL) : seed);
}

int roll(int x, int d){
	if( d == 0 ) return 0;
	int result=0;
	for(; x>0 ; x--){
		result += (rand() % d)+1;
	}
	return result;
}

int oneIn(int x){
	return !(rand() % (x < 1 ? 1 : x));
}