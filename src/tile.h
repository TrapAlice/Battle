#ifndef _TILE_H
#define _TILE_H


typedef struct tile_t{
	char ops;
	char self;
}tile_t;

tile_t* createTile(char, char, char);
void deleteTile(tile_t*);

#endif