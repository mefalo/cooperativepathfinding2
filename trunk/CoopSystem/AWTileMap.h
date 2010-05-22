/* Version 2.3 */

#ifndef __AWTILEMAPhINCLUDED__
#define __AWTILEMAPhINCLUDED__     (1)

#include "SDL.h"
#include "AWTileSet.h"

class AWTileMap {
private:
	int *map;
	AWTileSet *tileSet;
	int tilesAcross;
	int tilesDown;
	int screenWidth;
	int screenHeight;
public:
	AWTileMap::AWTileMap(AWTileSet *tS, int tA, int tD);
	void AWTileMap::set_square_contents(int tileX, int tileY, int tileNum);
	int  AWTileMap::get_square_contents(int tileX, int tileY);
	void AWTileMap::set_map_contents(int *array);
	void AWTileMap::draw_screen(int topLeftX, int topLeftY);
	void AWTileMap::draw_screen(int topLeftX, int topLeftY, SDL_Surface *onto);

};

#endif



