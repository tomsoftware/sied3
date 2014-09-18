#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <SDL.h>
#include <iostream>

#include "clGFXFile.h"
#include "clError.h"
#include "clMapFileReader.h"


int main(int argc, char* argv []);
void gameLoop(SDL_Renderer *renderer);
void loadResource(SDL_Renderer *renderer);
void loadMap(SDL_Renderer *renderer, const char * fileName);

struct ty_Animation
{
	clGFXFile::GFX_ObjectTexture *texture;
	clGFXFile::GFX_ObjectSurface *torso;
	int count;
};

clError m_error = clError("main");

clGFXFile::GFX_ObjectTexture txBuild;
clGFXFile::GFX_ObjectTexture txBack;
clGFXFile::GFX_ObjectTexture txSied;
clGFXFile::GFX_ObjectSurface txTorso;

SDL_Texture * mapPreview;

ty_Animation animWizzard;

SDL_Palette*palTorso;

int *m_map_AraeHeightObject = NULL;
int *m_map_AccessiblePlayerResources=NULL;
int m_mapWidth=0;
int m_mapHeight=0;

#endif