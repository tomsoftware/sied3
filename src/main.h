#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>


#define STRINGIFY(A) #A



#include <iostream>

#include "openGL.h"
#include "clGFXFile.h"
#include "clError.h"
#include "clMapFileReader.h"
#include "clGameObjects.h"
#include "clTexturesLoadHelper.h"
#include "clLandscapeTextures.h"
#include "clObjectTextures.h"
#include "clShader.h"
#include "buildings/clBuildingFactory.cpp"
#include "clScreen.h"

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
void drawMapBuildings(int posX, int posY);

bool checkForGlError(const char * errorText);
clError m_error = clError("main");

GLFWwindow* m_window;

clScreen m_Screen;
clBuildingFactory BuildingsFactory;

clTexturesLoadHelper::ty_TextureObject txBuild;
clTexturesLoadHelper::ty_TextureObject txBuildings[256];
clTexturesLoadHelper::ty_TextureObject txSied;
clTexturesLoadHelper::ty_TextureObject txTorso;

clLandscapeTextures * m_LandscapeText;
clObjectTextures * m_ObjectText;
clObjectTextures * m_BuldingsText;



//ty_Animation animWizzard;

struct ty_textureMapPos
{
	//- A texture in texture atlas
	float textureA_var1_X;
	float textureA_var1_Y;

	float textureA_var2_X;
	float textureA_var2_Y;

	//- B texture in texture atlas
	float textureB_var1_X;
	float textureB_var1_Y;

	float textureB_var2_X;
	float textureB_var2_Y;

	unsigned char /* clLandscapeTextures::enumTextureType */ textureType;
};

ty_textureMapPos * m_texturePos;
int m_texturePos_Size;



int AnimationID = 382; //368;
int BuildingID = 10;
int m_GameLoopCounter = 0;

struct tyBuilding //- 41
{
	//- size: 5 Byte
	unsigned char Type;
	unsigned int state;

	//- size: 9*4 Byte = 36
	unsigned int stack1;
	unsigned int stack2;
	unsigned int stack3;
	unsigned int stack4;
	unsigned int stack5;
	unsigned int stack6;
	unsigned int stack7;
	unsigned int stack8;
	unsigned int stack9;
};

struct tyMapObject //- Tree, stone, ...
{
	//- size: 2 Byte
	unsigned char Type;
	unsigned char state;

};

struct tyStack
{
	//- size: 4 Byte
	unsigned char ResourceType;
	unsigned char currentAmount;
	unsigned char outAmount; //- how many items are requestet to leave this stack
	unsigned char inAmount; //- how many items are requestet to add to this stack
};

struct tySettler
{
	unsigned char Type;
	unsigned int destination1; //- stack from
	unsigned int destination2; //- stack to
	unsigned char ResourceType; //- 0=nothing

};

union UNION_map_object {
	tyBuilding building;
	tyMapObject object;
	tyStack stack;

};


struct ty_mapLandscape
{

	//- drawing:
	//--       1 --- 4  
	//--      / \ B /
	//--     / A \ /   
	//--   (2) ---3 


	//- A texture in texture atlas
	int textureMapPosA;
	//- B texture in texture atlas
	int textureMapPosB;


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

};

ty_mapLandscape *m_map_landscape;
#ifndef NDEBUG 
int m_MarkerTextureTypeID;
int m_ErrorTextureTypeID;
#endif

absBuilding::tyBuilding *m_map_buildings;
int m_map_buildings_count;

unsigned int *m_map_AraeHeightObject = NULL;
unsigned int *m_map_AccessiblePlayerResources = NULL;
//unsigned int *m_map_AraeNeighbor = NULL;
int m_mapWidth=0;
int m_mapHeight=0;
//void drawObject(clTexturesLoadHelper::ty_TextureObject *texture, clTexturesLoadHelper::ty_TextureObject *torso, int x, int y, int scale = 1);

int m_mapPosX = 0;
int m_mapPosY = 50;
int m_MouseDownStartX = 0;
int m_MouseDownStartY = 0;

///<summery>mark special map-type</summery>
#ifndef NDEBUG 
int m_marker = -1;
bool m_useHeight = true;
#endif



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
//GLuint m_pixel_shader;
//GLuint m_shader_Program;

clShader m_map_shader;

//ProgramObject: GLhandle;
//VertexShaderObject: GLhandle;
//FragmentShaderObject: GLhandle;

#endif