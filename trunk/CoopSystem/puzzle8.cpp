#include <iostream>
#include "string.h"
#include "AStarNode.h"
#include "AStarPool.h"
#include "AStarList.h"
#include "AStarStack.h"
#include "defs.h"
#include "data.h"

#include "SDL.h"
#include "AWTileSet.h"
#include "AWTileMap.h"
#include "AWSprite.h"
#include "AWFont.cpp"
#include "AWMagazine.h"

#include "jCooperativeTable.h"

using namespace std;

// Each tile is 30x30 pixels
// The backdrop is 960x600 pixels, which is 32 across by 20 down
// (Note that this seems "backwards" in the declaration of the array)

locstruct STARTSTATE =  { 0,  0};
locstruct TARGETSTATE = { 30, 19};

AStarPool pool;
AStarList OPEN(STARTSTATE);
AStarList CLOSED;


AStarNode *DoAStar() {
	AStarNode *current, *kid, *pato, *patc;
	int k;
	int iterations = 0;

	while(OPEN.IsEmpty() == false) {
		current = OPEN.GetBestNode();
		iterations++;
		current->Print();
		if(current->SameTiles(TARGETSTATE)) {
			cout << "\nDONE! It took " << iterations << " iterations\n";
			return current;
		}
		current->Generate();

		for(k = 0; k < 8; k++) {
			kid = current->kids[k];
			if(kid == NULL) continue;

			pato = OPEN.PositionAlreadyThere(kid);
			patc = CLOSED.PositionAlreadyThere(kid);
			if(pato != NULL) {
				if(pato->gsteps <= kid->gsteps) {
					current->kids[k] = NULL;
					continue;
				} else {
					OPEN.Delete(pato);
				}
			} else if(patc != NULL) {
				if(patc->gsteps <= kid->gsteps) {
					current->kids[k] = NULL;
					continue;
				} else {
					patc->Update();
					CLOSED.Delete(patc);
				}
			}
			OPEN.Add(kid);
		}
		// Add current to CLOSED list
		CLOSED.Add(current);
	}
	return NULL;
}



// This MUST be extended, if the solution could be bigger!!
#define SOLUTIONSTEPS   (500)
AStarNode *solution[SOLUTIONSTEPS];

// This is a truly horrible way of getting the solution steps into an array
void LinearisePath(AStarNode *startpoint, AStarNode *arr[SOLUTIONSTEPS]) {

	int myMap[MAXY][MAXX];

		for(int i = 0; i < MAXY; i++)
	{
		for(int y = 0; y < MAXX; y++)
			myMap[i][y] = jCoop.getGraphAtTime(0)->myMap[i][y];
	}

	AStarNode *node;
	AStarStack s;
	int totalCost = 0;
	
	node = startpoint;
	s.Push(node);
	while(node->parent != NULL) {
		s.Push(node->parent);
		node = node->parent;
	}
	int i=0;
	while(!s.IsEmpty()) {
		node = s.Pop();
		totalCost += myMap[node->loc.y][node->loc.x];
		arr[i] = node;
		i++;
		if(i >= SOLUTIONSTEPS-1) break;
	}
	arr[i] = NULL;
	cout << "Finished linearising, there are " << i << " entries." << endl;
	cout << "The total cost was: " << totalCost << endl << endl;
}


void RandomMap() 
{

	//int row, col;
	//for(row=0; row < MAXY ; row++) {
	//		for(col=0; col < MAXX; col++) {
	//			int r = (rand() % 7) + 1;
	//			myMap[row][col] = r;
	//		}
	//}
}

	

int main( int argc, char *argv[] ) {
	jCoop.Init(25);

	AStarNode *goal;

	srand(3);
	RandomMap();

	int myMap[MAXY][MAXX];

		for(int i = 0; i < MAXY; i++)
	{
		for(int y = 0; y < MAXX; y++)
			myMap[i][y] = jCoop.getGraphAtTime(0)->myMap[i][y];
	}


	myMap[STARTSTATE.y][STARTSTATE.x] = 0;
	myMap[TARGETSTATE.y][TARGETSTATE.x] = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  	SDL_Surface* screen = SDL_SetVideoMode(960, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	atexit(SDL_Quit);

	AWFont *vera = new AWFont("vera.bmp", "vera.dat");

	// The AWTileSet constructor says:
	//    * Each tile is 30x30 pixels in size;
	//    * There is a 2 pixel border at the left
	//    * There is no border on the right
	//    * There is a 2 pixel border at the top
	//    * There is no border on the bottom
	AWTileSet *ts = new AWTileSet("astarblocks.bmp", 30, 30, 2, 0, 2, 0);
	ts->set_default_tile(0);

	// We arbitrarily say that our world is 960x600 pixels, each tile 
	//  is 30x30, so our tilemap is 32x20 tiles
	AWTileMap *tm = new AWTileMap(ts, 32, 20);

	// We use the array we declared above (myMap) to set up what each tile should look like
	//  In this program, they are all set to tile number 175. Try changing that to see what happens.
	tm->set_map_contents((int *)myMap);

	AWSprite *thing = new AWSprite("thing.bmp", 4);
	thing->set_transparent_colour(0, 0, 0);
	thing->set_auto_animate(300);
	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);

	AWMagazine *breadcrumbs = new AWMagazine(200, "crumb.bmp", 1);
	breadcrumbs->set_transparent_colour(255, 255, 255);


	goal = DoAStar();
	if(goal == NULL) {
		cout << "\nFAILED\n";
	} else {
		cout << "\nSUCCESS!!\n";
		goal->TracePath();
	}

	LinearisePath(goal, solution);

	// This is the game loop message handler
	SDL_Event event;
	int gameover = 0;
	Uint32 counter = 0, sol = 0;
	bool resetThing=false;

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
						case SDLK_r:
							resetThing = true;
							break;
					}
					break;
			}
    	}

		if(resetThing) {
			sol = 0;
			counter = 0;
			// Clear the breadcrumbs
			unsigned int bc;
			for(bc = 0; bc < breadcrumbs->size(); bc++) {
				breadcrumbs->get(bc)->inUse = false;
			}
			thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
			resetThing = false;
		}



		tm->draw_screen(0, 0);
		int X, Y;
		for(X=0; X < MAXX; X++) {
			for(Y=0; Y < MAXY; Y++) {
				if(X == STARTSTATE.x && Y == STARTSTATE.y) {
					vera->print_string("S", 9+X*30, 7+Y*30);
				} else if(X == TARGETSTATE.x && Y == TARGETSTATE.y) {
					vera->print_string("T", 9+X*30, 7+Y*30);
				} else {
					vera->print_integer(myMap[Y][X], 9+X*30, 7+Y*30);
				}
			}
		}
		
		// This uses the solution steps array to trace the path
		//  using the thing
		counter++;
		if(counter % 30 == 0) {
			sol++;
			AStarNode *node;
			if(sol < SOLUTIONSTEPS) {
				node = solution[sol];
				if(node != NULL) {
					thing->set_world_position(30*node->loc.x, 30*node->loc.y);
					// Show the crumb for this square
					AWBullet *crumb = breadcrumbs->allocate_a_bullet();
					if(crumb != NULL) crumb->set_world_position(30*node->loc.x, 30*node->loc.y);
				}
			}
		}

		breadcrumbs->update_everything();
		thing->update_everything();
		SDL_Flip(screen);
	}

	SDL_Quit();

	return 0;
}
