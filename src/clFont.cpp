#include "clFont.h"


clFont::clFont(SDL_Renderer * renderer)
{
	SDL_Surface * image = SDL_LoadBMP("image.bmp");

	imgFont = SDL_CreateTextureFromSurface(renderer, image);



}


clFont::~clFont()
{
}
