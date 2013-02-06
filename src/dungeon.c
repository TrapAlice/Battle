#include "dungeon.h"
#include "map.h"
#include "moonmem.h"
#include "rng.h"

dungeon_t* createDungeon(int maxFloors){
	dungeon_t* dungeon = malloc(sizeof(dungeon_t));
	dungeon->maxFloors = maxFloors;
	dungeon->floors = malloc(sizeof(map_t)*maxFloors);
	return( dungeon );
}

void deleteDungeon(dungeon_t* dungeon){
	int x;
	for( x=0; x<dungeon->maxFloors; ++x ){
		free(dungeon->floors[x]);
	}
	free(dungeon);
}

map_t* newDungeonFloor(dungeon_t* const dungeon){
	int x;
	for( x=0; x<dungeon->maxFloors; ++x ){
		if( !dungeon->floors[x] ){
			dungeon->floors[x] = createMap(between(30,50), between(30,50));
			if( x == dungeon->maxFloors-1){
				makeMap(dungeon->floors[x], between(5,15), between(2,5), between(6,10), 0);
				generateTreasure(dungeon->floors[x]);
				removeStairsDown(dungeon->floors[x]);
			} else {
				makeMap(dungeon->floors[x], between(5,15), between(2,5), between(6,10), 0);
			}
			return dungeon->floors[x];
		}
	}
	return( 0 );
}

map_t* getDungeonFloor(dungeon_t* const dungeon, int floorNumber){
	map_t* map = dungeon->floors[floorNumber];
	if( !map ){
		map = newDungeonFloor(dungeon);
	}
	return( map );
}