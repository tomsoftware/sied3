#ifndef clTexturesLoadHelper_H
#define clTexturesLoadHelper_H

#include "openGL.h"

#include "clGFXFile.h"
#include "clError.h"
#include "clGameObjects.h"
#include "clObjectTextures.h"
#include "clLandscapeTextures.h"

class clTexturesLoadHelper
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


		//static void loadLandscapeTextureFromGFX(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId);
		//static void loadLandscapeTextureFromGFX2x2(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId00, int gfxTextureId01, int gfxTextureId10, int gfxTextureId11, int forGfxTextureId0, int forGfxTextureId1);
		static void loadObjectTextureFromGFX( ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId, int gfxSharowId, int gfxFram);

		//static void copyGFXimage(unsigned int *dest, unsigned int * src, int posX, int posY, int  width, int  height, int  destWidth);

		static void load_game_objects(clObjectTextures *objectTextures, clGFXFile *gfxObjects);
		//void static load_game_buildings_roman(SDL_Renderer *renderer, clGFXFile &gfxObjects, clGFXFile::GFX_ObjectTexture txObjects[255]);
		static void clTexturesLoadHelper::load_map_patterns(clLandscapeTextures *objectTextures, clGFXFile *gfxLand);


	private:
		static clTexturesLoadHelper &getInstance();
		clError m_error = clError("clTexturesLoadHelper");
		
};




#endif