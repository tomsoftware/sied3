#ifndef CLFONT_H
#define CLFONT_H

#include <SDL.h>

class clFont
{
public:
	SDL_Texture *imgFont = NULL;

	clFont(SDL_Renderer * renderer);
	~clFont();
};



#endif