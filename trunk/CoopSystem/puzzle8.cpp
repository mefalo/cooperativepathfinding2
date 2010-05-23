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

#include "AWFont.cpp"

#include "Agent.h"

#include "jCooperativeTable.h"

using namespace std;

// Each tile is 30x30 pixels
// The backdrop is 960x600 pixels, which is 32 across by 20 down
// (Note that this seems "backwards" in the declaration of the array)

//locstruct STARTSTATE =  { 0,  0};
//locstruct TARGETSTATE = { 30, 19};
	AStarPool pool;
//AStarPool pool;
//AStarList OPEN(STARTSTATE);
//AStarList CLOSED;


//AStarNode *DoAStar() {
//	AStarNode *current, *kid, *pato, *patc;
//	int k;
//	int iterations = 0;
//
//	while(OPEN.IsEmpty() == false) {
//		current = OPEN.GetBestNode();
//		iterations++;
//		current->Print();
//		if(current->SameTiles(TARGETSTATE)) {
//			cout << "\nDONE! It took " << iterations << " iterations\n";
//			return current;
//		}
//
//		current->Generate();
//
//		for(k = 0; k < 8; k++) {
//			kid = current->kids[k];
//			if(kid == NULL) continue;
//
//			pato = OPEN.PositionAlreadyThere(kid);
//			patc = CLOSED.PositionAlreadyThere(kid);
//			if(pato != NULL) {
//				if(pato->gsteps <= kid->gsteps) {
//					current->kids[k] = NULL;
//					continue;
//				} else {
//					OPEN.Delete(pato);
//				}
//			} else if(patc != NULL) {
//				if(patc->gsteps <= kid->gsteps) {
//					current->kids[k] = NULL;
//					continue;
//				} else {
//					patc->Update();
//					CLOSED.Delete(patc);
//				}
//			}
//			OPEN.Add(kid);
//		}
//		// Add current to CLOSED list
//		CLOSED.Add(current);
//	}
//	return NULL;
//}
//
//

// This MUST be extended, if the solution could be bigger!!
//#define SOLUTIONSTEPS   (500)
//AStarNode *solution[SOLUTIONSTEPS];

 //This is a truly horrible way of getting the solution steps into an array
//void LinearisePath(AStarNode *startpoint, AStarNode *arr[SOLUTIONSTEPS]) 
//{
//
//	AStarNode *node;
//	AStarStack s;
//	int totalCost = 0;
//	
//	node = startpoint;
//	s.Push(node);
//	while(node->parent != NULL) 
//	{
//		s.Push(node->parent);
//		node = node->parent;
//	}
//	int i=0;
//	while(!s.IsEmpty()) {
//		node = s.Pop();
//		totalCost += jCoop.getGraphAtTime(0)->myMap[node->loc.y][node->loc.x];
//		arr[i] = node;
//		i++;
//		if(i >= SOLUTIONSTEPS-1) break;
//	}
//	arr[i] = NULL;
//	cout << "Finished linearising, there are " << i << " entries." << endl;
//	cout << "The total cost was: " << totalCost << endl << endl;
//}
//

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
	AStarNode *goal;
	const int NUMAGENTS = 5;

	Agent* agent[NUMAGENTS];

	srand(3);
	//RandomMap();



	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  	SDL_Surface* screen = SDL_SetVideoMode(960, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	atexit(SDL_Quit);

		for( int i = 0; i < NUMAGENTS; i++)
	{
		agent[i] = new Agent();
	}

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
	tm->set_map_contents((int *)jCoop.getGraphAtTime(0)->myMap);

	/*AWSprite *thing = new AWSprite("thing.bmp", 4);
	thing->set_transparent_colour(0, 0, 0);
	thing->set_auto_animate(300);
	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);

	AWMagazine *breadcrumbs = new AWMagazine(200, "crumb.bmp", 1);
	breadcrumbs->set_transparent_colour(255, 255, 255);*/

	for( int i = 0; i < NUMAGENTS; i++)
		agent[i]->DoAStar();
	//goal = DoAStar();

		for( int i = 0; i < NUMAGENTS; i++)
		{
			if(agent[i]->getPathGoal() == NULL){
				cout << "\n Agent " << i << "FAILED\n";
			} else
			{
				cout<< "\n Agent " << i << "\nSUCCESS!!\n";
				agent[i]->TracePath();
		}
		}

	for( int i = 0; i < NUMAGENTS; i++)
				agent[i]->LinearisePath();
//	LinearisePath(goal, solution);

	// This is the game loop message handler
	SDL_Event event;
	int gameover = 0;

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
			for( int i = 0; i < NUMAGENTS; i++)
			agent[i]->resetCrumbs();
		//	sol = 0;
		//	counter = 0;
		//	// Clear the breadcrumbs
		//	unsigned int bc;
		//	for(bc = 0; bc < breadcrumbs->size(); bc++) {
		//		breadcrumbs->get(bc)->inUse = false;
		//	}
		//	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
		//	resetThing = false;
		}



		tm->draw_screen(0, 0);
		for( int i = 0; i < NUMAGENTS; i++){

		int X, Y;
		for(X=0; X < MAXX; X++) {
			for(Y=0; Y < MAXY; Y++) {
				if(X == agent[i]->STARTSTATE.x && Y == agent[i]->STARTSTATE.y) {
					vera->print_string("S", 9+X*30, 7+Y*30);
				} else if(X == agent[i]->TARGETSTATE.x && Y == agent[i]->TARGETSTATE.y) {
					vera->print_string("T", 9+X*30, 7+Y*30);
				} else {
					vera->print_integer(jCoop.getGraphAtTime(0)->myMap[Y][X], 9+X*30, 7+Y*30);
				}
			}
		}
		}
		
		for( int i = 0; i < NUMAGENTS; i++)
				agent[i]->Walk();

		SDL_Flip(screen);
	}

	SDL_Quit();

	return 0;
}
