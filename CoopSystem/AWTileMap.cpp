/* Version 2.3 */

#include "AWTileMap.h"

AWTileMap::AWTileMap(AWTileSet *tS, int tA, int tD) {
	map = new int [tA*tD];
	tilesAcross = tA;
	tilesDown = tD;
	int a, d;
	for(a=0; a < tA; a++) {
		for(d=0; d < tD; d++) {
			map[a+(d*tilesAcross)] = 0;
		}
	}
	tileSet = tS;
	SDL_Surface *screen = SDL_GetVideoSurface();
	screenWidth  = screen->w;
	screenHeight = screen->h;
}

void AWTileMap::set_square_contents(int tileX, int tileY, int tileNum) {
	if(tileNum >= tileSet->howManyTiles) tileNum = tileSet->defaultTile;
	if(tileNum < 0) tileNum = tileSet->defaultTile;
	map[tileX+(tileY*tilesAcross)] = tileNum;

}

int AWTileMap::get_square_contents(int tileX, int tileY){
	return map[tileX+(tileY*tilesAcross)];
}

void AWTileMap::set_map_contents(int *array) {
	int c, r, i;
	int tileNum;
	for(r=0; r < tilesDown; r++) {
		for(c=0; c < tilesAcross; c++) {
			i = c + (r*tilesAcross);
			tileNum = array[i];
			if(tileNum >= tileSet->howManyTiles) tileNum = tileSet->defaultTile;
			if(tileNum < 0) tileNum = tileSet->defaultTile;
			map[c + (r*tilesAcross)] = tileNum;
		}
	}
}


void AWTileMap::draw_screen(int topLeftX, int topLeftY, SDL_Surface *onto) {
	int mapOriginX, mapOriginY;
	int tileOffsetX, tileOffsetY;

	mapOriginX = topLeftX / tileSet->tileWidth;
	mapOriginY = topLeftY / tileSet->tileHeight;
	tileOffsetX = topLeftX % tileSet->tileWidth;
	tileOffsetY = topLeftY % tileSet->tileHeight;

	int xCoord = -tileOffsetX, yCoord = -tileOffsetY;
	int row, col;
	int whichTile;
	for(row = mapOriginY; yCoord < screenHeight; row++) {
		xCoord = -tileOffsetX;
		for(col = mapOriginX; xCoord < screenWidth; col++) {
			if(col < tilesAcross && row < tilesDown) {
				whichTile = map[col+(row*tilesAcross)];
			} else {
				whichTile= tileSet->defaultTile;
			}
			tileSet->draw(whichTile, xCoord, yCoord, onto);
			xCoord += tileSet->tileWidth;
		}
		yCoord += tileSet->tileHeight;
	}
}

// By default, we draw onto the Video Surface
void AWTileMap::draw_screen(int topLeftX, int topLeftY) {
	SDL_Surface *onto = SDL_GetVideoSurface();

	draw_screen(topLeftX, topLeftY, onto);
}












