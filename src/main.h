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
void drawMap(SDL_Renderer *renderer, int x, int y);

struct ty_Animation
{
	clGFXFile::GFX_ObjectTexture *texture;
	clGFXFile::GFX_ObjectSurface *torso;
	int count;
};

clError m_error = clError("main");

SDL_Window *m_sdl_window;

clGFXFile::GFX_ObjectTexture txBuild;
clGFXFile::GFX_ObjectTexture txLandscape[255];
clGFXFile::GFX_ObjectTexture txSied;
clGFXFile::GFX_ObjectSurface txTorso;

SDL_Texture * mapPreview;

ty_Animation animWizzard;

SDL_Palette*palTorso;

int AnimationID = 382; //368;
int BuildingID = 10;
int m_GameLoopCounter = 0;

unsigned int *m_map_AraeHeightObject = NULL;
unsigned int *m_map_AccessiblePlayerResources = NULL;
int m_mapWidth=0;
int m_mapHeight=0;
void drawObject(SDL_Renderer *renderer, clGFXFile::GFX_ObjectTexture *texture, clGFXFile::GFX_ObjectSurface *torso, int x, int y, int scale=1);

int m_mapPosX = 0;
int m_mapPosY = 0;
int m_MouseDownStartX = 0;
int m_MouseDownStartY = 0;

clGFXFile gfxSied12;
clGFXFile gfxSied11;
clGFXFile gfxSied10;
clGFXFile gfxAnimation;
clGFXFile gfxBuilding;


#endif