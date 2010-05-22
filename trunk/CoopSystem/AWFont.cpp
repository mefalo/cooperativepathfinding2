#include "stdio.h"
#include "SDL.h"
#include <fstream>

using namespace std;

#ifdef _MSC_VER
#define SNPRINTF	_snprintf_s
#else
#define SNPRINTF	snprintf
#endif


class AWFont {
private:
	char charWidths[256];
	SDL_Surface *fontSurface;


public:
	AWFont::AWFont(char *bmpName, char *widthsName) {
		int transparentColour;
		/* load bitmap to temp surface */
		SDL_Surface* temp = SDL_LoadBMP(bmpName);
		/* convert bitmap to display format */
		fontSurface = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		SDL_Surface *onto = SDL_GetVideoSurface();
		transparentColour = SDL_MapRGB(onto->format, 0, 0, 0);
		SDL_SetColorKey(fontSurface, (SDL_SRCCOLORKEY | SDL_RLEACCEL), transparentColour);	
		
		ifstream widthsFile(widthsName, ios::in | ios::binary);
		widthsFile.read(charWidths, 256);
		if (!widthsFile) {
			// Something wrong with the widths file, so we
			//  default every letter to be 16 pixels wide
			int i;
			for(i=0; i < 256; i++) {
				charWidths[i] = 16;
			}
		}
	}


	int AWFont::print_string(char *string, int X, int Y, SDL_Surface *onto) {
		SDL_Rect from;
		SDL_Rect to;
		int c=0;
		char ch;

		to.x = X;
		to.y = Y;
		to.w = 16;
		to.h = 16;
		from.w = 16;
		from.h = 16;
		while(string[c] != 0) {
			ch = string[c];
			from.x = 16 * (ch % 16);
			from.y = 16 * (ch / 16);
			SDL_BlitSurface(fontSurface, &from, onto, &to);
			to.x = to.x + charWidths[ch];
			c++;
		}
		return to.x;
	}

	int AWFont::print_string(char *string, int X, int Y) {
		SDL_Surface *onto = SDL_GetVideoSurface();
		return this->print_string(string, X, Y, onto);
	}


	int AWFont::print_integer(int number, int X, int Y, SDL_Surface *onto) {
		char buffer[32];
		SNPRINTF(buffer, 32, "%d", number);
		return print_string(buffer, X, Y, onto);
	}

	int AWFont::print_integer(int number, int X, int Y) {
		char buffer[32];
		SNPRINTF(buffer, 32, "%d", number);
		return print_string(buffer, X, Y);
	}
};











