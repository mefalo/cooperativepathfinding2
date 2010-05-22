/* Version 2.3 */

#ifndef __AWTILEcppINCLUDED__
#define __AWTILEcppINCLUDED__     (1)

#include "SDL.h"

class AWTile {
private:
	SDL_Rect tileRect;
	SDL_Surface *tileSetSurface;

public:
	AWTile::AWTile() { ; }

	void AWTile::set_surface(SDL_Surface *tSS, SDL_Rect tR) {
		tileSetSurface = tSS; 
		tileRect = tR;
	}

	void AWTile::draw(int screenX, int screenY, SDL_Surface *onto) {
		SDL_Rect dest;
		dest = tileRect;
		dest.x = screenX;
		dest.y = screenY;
		SDL_BlitSurface(tileSetSurface, &tileRect, onto, &dest);
	}

};

#endif
