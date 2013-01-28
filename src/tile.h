#ifndef _TILE_H
#define _TILE_H


typedef struct tile_t{
	char self;
	int x;
	int y;
	char blocked;
	char blockSight;
	char visible;
	char explored;
}tile_t;

tile_t* createTile(char, int, int, char, char);
void deleteTile(tile_t*);

#endif