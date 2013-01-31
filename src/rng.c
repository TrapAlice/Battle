#include "rng.h"
#include <time.h>
#include <stdlib.h>

void initSeed(int seed){
	srand(seed==0 ? time(NULL) : seed);
}

int roll(int x, int d){
	int result=0;
	if( !x || !d ) return 0;
	
	while(x--){
		result += (rand() % d)+1;
	}
	return( result );
}

int oneIn(int x){
	return( !(rand() % (x < 1 ? 1 : x)) );
}

int between(int min, int max){
	return( (rand() % (max-min))+min );
}