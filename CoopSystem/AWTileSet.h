/* Version 2.3 */

#ifndef __AWTILESEThINCLUDED__

#define __AWTILESEThINCLUDED__  (1)

#include "SDL.h"
#include "AWTile.cpp"

class AWTileSet {
private:
	SDL_Surface *surface;
	AWTile *tiles;
	 
public:
	int tileWidth;
	int tileHeight;
	int defaultTile;
	int howManyTiles;
	int leftBorder, rightBorder, topBorder, botBorder;

	AWTileSet::AWTileSet(char *bmpName, int tileW, int tileH);
	AWTileSet::AWTileSet(char *bmpName, int tileW, int tileH, int leftB, int rightB, int topB, int botB);
	void AWTileSet::draw(int tileNumber, int screenX, int screenY, SDL_Surface *onto);
	void AWTileSet::set_default_tile(int dT);
};

#endif


