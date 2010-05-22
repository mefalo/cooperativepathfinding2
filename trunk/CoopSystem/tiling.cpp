// Tilemapped background instead of a bitmap background
//

#include "SDL.h"
#include "AWTileSet.h"
#include "AWTileMap.h"
#include "AWSprite.h"

#define CARSPEED   (2)

int tilingmain ( int argc, char *argv[] ) {
	bool movingForward = false;
	bool turningRight  = false;
	int xMove = 0, yMove = 0;
	int frame = 0;
	int cf = 0;

	int X = 0;
	int Y = 0;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  	SDL_Surface* screen = SDL_SetVideoMode(960, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

	// The AWTileSet constructor says:
	//    * Each tile is 30x30 pixels in size;
	//    * There is a 2 pixel border at the left
	//    * There is no border on the right
	//    * There is a 2 pixel border at the top
	//    * There is no border on the bottom
	AWTileSet *ts = new AWTileSet("astarblocks.bmp", 30, 30, 2, 0, 2, 0);
	ts->set_default_tile(0);

	// We arbitrarily say that our world is 960x480 pixels, each tile 
	//  is 30x30, so our tilemap is 32x20 tiles
	AWTileMap *tm = new AWTileMap(ts, 32, 20);

	// Each tile is 30x30 pixels
	// The backdrop is 960x600 pixels, which is 32 across by 20 down
	// (Note that this seems "backwards" in the declaration of the array)
	int myMap[20][32] = { 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 7, 7, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }, 
						{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }
		};

	// We use the array we declared above (myMap) to set up what each tile should look like
	//  In this program, they are all set to tile number 175. Try changing that to see what happens.
	tm->set_map_contents((int *)myMap);

	// This is the game loop message handler
	//  It doesn't actually do anything!
	SDL_Event event;
	int gameover = 0;
	while (!gameover) {
    	/* look for an event */
    	if (SDL_PollEvent(&event)) {
    	  /* an event was found */
    	  switch (event.type) {
        	/* close button clicked */
        	case SDL_QUIT:
        	  gameover = 1;
        	  break;

        	/* handle the keyboard */
        	case SDL_KEYDOWN:
        	  switch (event.key.keysym.sym) {
            	case SDLK_ESCAPE:
            	case SDLK_q:
            	  gameover = 1;
            	  break;
            	case SDLK_UP:
            	  movingForward = true;
            	  break;
				case SDLK_RIGHT:
				  turningRight = true;
				  break;
			  }
        	  break;
        	case SDL_KEYUP:
        	  switch (event.key.keysym.sym) {
            	case SDLK_UP:
            	  movingForward = false;
            	  break;
				case SDLK_RIGHT:
				  turningRight = false;
				  break;
			  }
        	  break;
    	  }
    	}


		if(movingForward) {
			switch(cf) {
				case 0: // north
					xMove = 0;
					yMove = -CARSPEED;
					break;
				case 1: // north-east
					xMove = +CARSPEED;
					yMove = -CARSPEED;
					break;
				case 2: // east
					xMove = +CARSPEED;
					yMove = 0;
					break;
				case 3: // south-east
					xMove = +CARSPEED;
					yMove = +CARSPEED;
					break;
				case 4: // south
					xMove = 0;
					yMove = +CARSPEED;
					break;
				case 5: // south-west
					xMove = -CARSPEED;
					yMove = +CARSPEED;
					break;
				case 6: // west
					xMove = -CARSPEED;
					yMove = 0;
					break;
				case 7: // north-west
					xMove = -CARSPEED;
					yMove = -CARSPEED;
					break;
			}
		} else { // movingForward is false, so stop
			xMove = 0;
			yMove = 0;
		}

    	/* draw the background based on where the car is*/
		tm->draw_screen(0, 0);
		

		SDL_Flip(screen);
		SDL_Delay(5);
	}

	SDL_Quit();

	return 0;
}
