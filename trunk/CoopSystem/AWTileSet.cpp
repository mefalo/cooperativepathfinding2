/* Version 2.3 */

#include "SDL.h"
#include "AWTileSet.h"


AWTileSet::AWTileSet(char *bmpName, int tileW, int tileH, int leftB, int rightB, int topB, int botB) {
	SDL_Surface* temp = SDL_LoadBMP(bmpName);
	surface = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	leftBorder  = leftB;
	rightBorder = rightB;
	topBorder   = topB;
	botBorder   = botB;

	printf("tW=%d\tleftB=%d\trightB=%d\n\n", tileW, leftBorder, rightBorder);
	printf("tH=%d\ttopB=%d\tbotB=%d\n\n", tileH, topBorder, botBorder);
	fflush(stdout);
	int across = surface->w/(tileW+leftBorder+rightBorder);
	int down   = surface->h/(tileH+topBorder+botBorder);
	howManyTiles = across*down;

	printf("across=%d\tdown=%d\thowMany=%d\n\n", across, down, howManyTiles);

	tiles = new AWTile[howManyTiles];
	tileWidth = tileW;
	tileHeight = tileH;
	defaultTile = 0;

	int acc, dow, tileNum;
	SDL_Rect tileRect;
	tileRect.w = tileWidth;
	tileRect.h = tileHeight;
	for(acc = 0; acc < across; acc++) {
		for(dow = 0; dow < down; dow++) {
			tileRect.x = leftBorder + acc*(tileWidth+rightBorder+leftBorder);
			tileRect.y = topBorder  + dow*(tileHeight+botBorder+topBorder);
			tileNum = dow*across + acc;
			printf("num=%d\tx=%d\ty=%d\n\n", tileNum, tileRect.x, tileRect.y);
			tiles[tileNum].set_surface(surface, tileRect);
		}
	}
}


void AWTileSet::draw(int tileNumber, int screenX, int screenY, SDL_Surface *onto) {
	tiles[tileNumber].draw(screenX, screenY, onto);
}

void AWTileSet::set_default_tile(int dT) {
	defaultTile = dT;
}


