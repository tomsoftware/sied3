#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "clGFXFile.h"
#include "clError.h"
#include "clMapFileReader.h"
#include "clGameObjects.h"
#include "clOpenGLTexturesHelper.h"
#include "clLandscapeTextures.h"

struct ty_Animation
{
	clOpenGLTexturesHelper::ty_TextureObject *texture;
	clOpenGLTexturesHelper::ty_TextureObject *torso;
	int count;
};





void toPerspective();
void toOrtho();

void initGL();
int  initGLUT(int argc, char **argv);


bool mouseLeftDown = false;
bool mouseRightDown = false;
bool mouseMiddleDown = false;
float mouseX=0;
float mouseY=0;
float cameraAngleX=0;
float cameraAngleY=0;
float cameraDistance = 10.0f;
int drawMode=0;// 0:fill, 1: wireframe, 2:points
int screenWidth = 1024;
int screenHeight = 640;



int main(int argc, char* argv []);
void gameLoop();
void setUpCam();

bool initWindow();
void unloadWindow();


void loadResource();
void loadMap( const char * fileName, clMapFileReader::enum_map_folders mapType);
void drawMap(int x, int y);
void drawMapObjects(int posX, int posY);

bool checkForGlError(const char * errorText);
clError m_error = clError("main");

GLFWwindow* m_window;


clOpenGLTexturesHelper::ty_TextureObject txBuild;
//clOpenGLTexturesHelper::ty_TextureObject txLandscape[256];
clOpenGLTexturesHelper::ty_TextureObject txObjects[256];
clOpenGLTexturesHelper::ty_TextureObject txBuildings[256];
clOpenGLTexturesHelper::ty_TextureObject txSied;
clOpenGLTexturesHelper::ty_TextureObject txTorso;

clLandscapeTextures * m_LandscapeText;

//SDL_Texture * mapPreview;

ty_Animation animWizzard;

//SDL_Palette*palTorso;

int AnimationID = 382; //368;
int BuildingID = 10;
int m_GameLoopCounter = 0;

struct ty_mapLandscape
{

	//- drawing:
	//--       1 --- 4  
	//--      / \ B /
	//--     / A \ /   
	//--   (2) ---3 

	//- A texture in texture atlas
	float textureA_var1_X;
	float textureA_var1_Y;
	//- B texture in texture atlas
	float textureB_var1_X;
	float textureB_var1_Y;

	//- A texture in texture atlas
	float textureA_var2_X;
	float textureA_var2_Y;
	//- B texture in texture atlas
	float textureB_var2_X;
	float textureB_var2_Y;

	//- point (2) is the main point of this feld
	int AraeType; //- type of point
	int AraeHight;  //- height of point

	clLandscapeTextures::enumTextureType textureType;
};

ty_mapLandscape *m_map_landscape;


unsigned int *m_map_AraeHeightObject = NULL;
unsigned int *m_map_AccessiblePlayerResources = NULL;
unsigned int *m_map_AraeNeighbor = NULL;
int m_mapWidth=0;
int m_mapHeight=0;
//void drawObject(clOpenGLTexturesHelper::ty_TextureObject *texture, clOpenGLTexturesHelper::ty_TextureObject *torso, int x, int y, int scale = 1);

int m_mapPosX = 0;
int m_mapPosY = 220;
int m_MouseDownStartX = 0;
int m_MouseDownStartY = 0;

int m_marker = -1;


clGFXFile gfxSied12;
clGFXFile gfxSied11;
clGFXFile gfxSied10;
clGFXFile gfxAnimation;
clGFXFile gfxBuilding;



static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
static void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);



#endif