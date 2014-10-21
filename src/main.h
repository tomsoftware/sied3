#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>


#define STRINGIFY(A) #A


#define GLEW_STATIC


// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#pragma comment(lib, "glew32.lib")

#include <iostream>

#include "clGFXFile.h"
#include "clError.h"
#include "clMapFileReader.h"
#include "clGameObjects.h"
#include "clTexturesLoadHelper.h"
#include "clLandscapeTextures.h"
#include "clObjectTextures.h"
#include "clShader.h"

/*
struct ty_Animation
{
	clTexturesLoadHelper::ty_TextureObject *texture;
	clTexturesLoadHelper::ty_TextureObject *torso;
	int count;
};
*/

enum enumGradient
{
	GRADIENT_NONE,
	GRADIENT_NORTH,
	GRADIENT_NORTH_EAST,
	GRADIENT_EAST,
	GRADIENT_EAST_SOUTH,
	GRADIENT_SOUTH,
	GRADIENT_SOUTH_WEST,
	GRADIENT_WEST,
	GRADIENT_WEST_NORTH,
};

float GRADIENT_COLOR[] = { 0.5f, 0.3f, 0.3f, 0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f };

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
int drawMode=0;// 0:fill, 1:wireframe, 2:points
int screenWidth = 1024;
int screenHeight = 640;



int main(int argc, char* argv []);
void gameLoop();
void setUpCam();

bool initGLContext();
void unloadWindow();

void loadShader();

void loadResource();
void loadMap( const char * fileName, clMapFileReader::enum_map_folders mapType);
void drawMap(int x, int y);
void drawMapObjects(int posX, int posY);

bool checkForGlError(const char * errorText);
clError m_error = clError("main");

GLFWwindow* m_window;


clTexturesLoadHelper::ty_TextureObject txBuild;
clObjectTextures::ty_TextureObject *txObjects;
clTexturesLoadHelper::ty_TextureObject txBuildings[256];
clTexturesLoadHelper::ty_TextureObject txSied;
clTexturesLoadHelper::ty_TextureObject txTorso;

clLandscapeTextures * m_LandscapeText;
clObjectTextures * m_ObjectText;



//ty_Animation animWizzard;



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
	unsigned char AraeType; //- type of point

	unsigned char AraeHeight1;  //- height of point 1
	unsigned char AraeHeight2;  //- height of point 2 (Main)
	unsigned char AraeHeight3;  //- height of point 3
	unsigned char AraeHeight4;  //- height of point 4

	//- gradient for shadow
	unsigned char gradientA1; // Of point 1
	unsigned char gradientA2; // Of point 2 (Main)
	unsigned char gradientA3; // Of point 3
	unsigned char gradientA4; // Of point 4

	unsigned char /* clLandscapeTextures::enumTextureType */ textureType;
};

ty_mapLandscape *m_map_landscape;
ty_mapLandscape m_markerType; 
ty_mapLandscape m_ErrorType;

unsigned int *m_map_AraeHeightObject = NULL;
unsigned int *m_map_AccessiblePlayerResources = NULL;
//unsigned int *m_map_AraeNeighbor = NULL;
int m_mapWidth=0;
int m_mapHeight=0;
//void drawObject(clTexturesLoadHelper::ty_TextureObject *texture, clTexturesLoadHelper::ty_TextureObject *torso, int x, int y, int scale = 1);

int m_mapPosX = 0;
int m_mapPosY = 220;
int m_MouseDownStartX = 0;
int m_MouseDownStartY = 0;

int m_marker = -1;
bool m_useHeight = true;

//clGFXFile gfxSied12;
//clGFXFile gfxSied11;
//clGFXFile gfxSied10;
//clGFXFile gfxAnimation;
//clGFXFile gfxBuilding;



static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
static void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);

enumGradient getGradientA(unsigned char H1, unsigned char H2, unsigned char H3);
enumGradient getGradientB(unsigned char H1, unsigned char H2, unsigned char H3);

//void initShader();
//void rendershadertest();

//GLuint m_brightness; //- shader Variable
//GLuint m_pixle_shader;
//GLuint m_shader_Program;

clShader m_map_shader;

//ProgramObject: GLhandle;
//VertexShaderObject: GLhandle;
//FragmentShaderObject: GLhandle;

#endif