#ifndef CLOPENGLTEXTURESHELPER_H
#define CLOPENGLTEXTURESHELPER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "clGFXFile.h"
#include "clError.h"
#include "clGameObjects.h"

class clOpenGLTexturesHelper
{
	public:

		enum enumTextureType
		{
			TEXTURE_TYPE_PLANE = 0,
			TEXTURE_TYPE_2x2_HEXAGON = 1
		};

		struct ty_TextureObject
		{
			GLuint texture = NULL;
			int xRel = 0;
			int yRel = 0;
			int width = 0;
			int height = 0;
			int forTextureId0 = 0;
			int forTextureId1 = 0;
			enumTextureType textType = TEXTURE_TYPE_PLANE;
		};


		static void loadLandscapeTextureFromGFX(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId);
		static void loadLandscapeTextureFromGFX2x2(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId00, int gfxTextureId01, int gfxTextureId10, int gfxTextureId11, int forGfxTextureId0, int forGfxTextureId1);
		static void loadObjectTextureFromGFX(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId, int gfxSharowId, int gfxFram);

		static void copyGFXimage(unsigned int *dest, unsigned int * src, int posX, int posY, int  width, int  height, int  destWidth);

		static void load_game_objects(clGFXFile &gfxObjects, clOpenGLTexturesHelper::ty_TextureObject txObjects[255]);
		//void static load_game_buildings_roman(SDL_Renderer *renderer, clGFXFile &gfxObjects, clGFXFile::GFX_ObjectTexture txObjects[255]);

		static bool checkForGlError(const char * errorText);


	private:
		clOpenGLTexturesHelper();
		~clOpenGLTexturesHelper();
		GLuint getFreeTexture();
		static clOpenGLTexturesHelper &getInstance();
		clError m_error = clError("clOpenGLTexturesHelper");
		
};




#endif