/* Version 2.3 */


#ifndef __AWSPLASHSCREENINCLUDED__

#define __AWSPLASHSCREENINCLUDED__  (1)
// An AWSplashScreen is used for welcome screens, game over screens and 
// level transition screens. I'm sure that there could be many other uses.

#include "SDL.h"

class AWSplashScreen {

private:
	SDL_Rect ssRect;

public:
	SDL_Surface *splash;

	AWSplashScreen(char *bmpName) {

		// Load background
		SDL_Surface* temp = SDL_LoadBMP(bmpName);
		splash = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		ssRect.x = 0;
		ssRect.y = 0;
		ssRect.w = splash->w;
		ssRect.h = splash->h;
	}

	AWSplashScreen::~AWSplashScreen(void) {
		SDL_FreeSurface(splash);
	}
	
	
	// These eventloop_ functions are for simple adaptation of the
	//  AWSplashScreen class. For most purposes you should be able
	//  to get some more specific behaviour by filling in something
	//  here..
	virtual void eventloop_pre(SDL_Surface *screen) {
		; // Do nothing
	}
	virtual void eventloop_beginning(SDL_Surface *screen) {
		; // Do nothing
	}
	virtual void eventloop_before_flip(SDL_Surface *screen) {
		; // Do nothing
	}
	virtual void eventloop_post(SDL_Surface *screen) {
		; // Do nothing
	}

	void AWSplashScreen::display(Uint32 seconds) {
		SDL_Event event;
		Uint32 currentTicks;
		Uint32 startTicks = SDL_GetTicks();
		bool exitLoop = false;
		SDL_Surface *screen;

		screen = SDL_GetVideoSurface();
		// Before doing *ANYTHING*, get rid of any
		//  events that are in the queue. We just
		//  need to throw them away...
		while(SDL_PollEvent(&event)) { 
			; // Do nothing
		}
		
		eventloop_pre(screen);
		
		while(!exitLoop) {
			eventloop_beginning(screen);
			/* look for an event */
    		if(SDL_PollEvent(&event)) {
    			/* an event was found */
    			switch (event.type) {
        			/* close button clicked */
        			case SDL_QUIT:
        				exitLoop = true;
        				break;
        				/* handle the keyboard */
       				case SDL_KEYDOWN:
						exitLoop = true;
						break;
					case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
						exitLoop = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						exitLoop = true;
						break;
				}
    		}

			SDL_BlitSurface(splash, &ssRect, screen, NULL);
			eventloop_before_flip(screen);
			SDL_Flip(screen);

			currentTicks = SDL_GetTicks();
			if(seconds != 0 && (currentTicks - startTicks)/1000 >= seconds) {
				exitLoop = true;
			}
			// Wait 1/100th of a second so as to not hog the CPU!
			SDL_Delay(10);
		}
		
		eventloop_post(screen);
	}

};


#endif

