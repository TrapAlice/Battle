#ifndef _DUNGEON_H
#define _DUNGEON_H

struct map_t;

typedef struct dungeon_t{
	int            maxFloors;
	struct map_t **floors;
}dungeon_t;

dungeon_t*     createDungeon   ( int );
void           deleteDungeon   ( dungeon_t* );
struct map_t*  newDungeonFloor ( dungeon_t* );
struct map_t*  getDungeonFloor ( const dungeon_t*, int );

#endif