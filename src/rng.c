#include "rng.h"
#include <time.h>
#include <stdlib.h>

void RNG_init(int seed){
	srand((seed==0)? time(NULL) : seed);
}

int RNG_roll(int x, int d){
	if( d == 0 ) return 0;
	int result=0;
	for(; x>0 ; x--){
		result += (rand() % d)+1;
	}
	return result;
}