#include "clOpenGLTexturesHelper.h"



clOpenGLTexturesHelper::clOpenGLTexturesHelper()
{
}


clOpenGLTexturesHelper::~clOpenGLTexturesHelper()
{
	//- todo! glDeleteTextures();
}

//-------------------------------------//
/*
void clOpenGLTexturesHelper::copyGFXimage(unsigned int *dest, unsigned int * src, int posX, int posY, int  width, int  height, int  destWidth)
{
	unsigned int * srcP = src;
	unsigned int * destP = dest + destWidth * posY;

	for (int y = 0; y < height; y++)
	{
		unsigned int * curDestP = destP + destWidth * y + posX;

		for (int x = width; x > 0; x--)
		{
			*curDestP = *srcP;
			curDestP++;
			srcP++;
		}
	}
}
*/
//-------------------------------------//
/*
GLuint clOpenGLTexturesHelper::getFreeTexture()
{
	GLuint texID=NULL;

	//- Create one OpenGL texture
	glGenTextures(1, &texID);

	//- "Bind" the newly created texture
	glBindTexture(GL_TEXTURE_2D, texID);

	return texID;
}
*/

//-------------------------------------//
clOpenGLTexturesHelper &clOpenGLTexturesHelper::getInstance()
{
	static clOpenGLTexturesHelper instance;

	return instance;
}

//-------------------------------------//
/*
void clOpenGLTexturesHelper::loadLandscapeTextureFromGFX2x2(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId00, int gfxTextureId01, int gfxTextureId10, int gfxTextureId11, int forGfxTextureId0, int forGfxTextureId1)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;

	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId00);


	int w = gfxTexture.width;
	int h = gfxTexture.height;


	//- create tmp image 2x2
	unsigned int *imgData = new unsigned int[w * 2 * h * 2];

	//- copy image to tmp buffer
	copyGFXimage(imgData, gfxTexture.imageRGBA, 0, 0, w, h, w * 2);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	//- Next part
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId01);
	//- copy image to tmp buffer
	copyGFXimage(imgData, gfxTexture.imageRGBA, w, 0, w, h, w * 2);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);



	//- Next part
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId10);
	//- copy image to tmp buffer
	copyGFXimage(imgData, gfxTexture.imageRGBA, 0, h, w, h, w * 2);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	//- Next part
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId11);
	//- copy image to tmp buffer
	copyGFXimage(imgData, gfxTexture.imageRGBA, w, h, w, h, w * 2);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);



	//- Create one OpenGL texture
	dest->texture = getInstance().getFreeTexture();

	//-	Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w * 2, h * 2, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, imgData);
	checkForGlError("glTexImage2D()");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//- free temp data
	delete[] imgData;


	//- output
	dest->height = 2 * h;
	dest->height = 2 * w;
	dest->xRel = 0;
	dest->yRel = 0;

	dest->forTextureId0 = forGfxTextureId0;
	dest->forTextureId1 = forGfxTextureId1;

	dest->textType = TEXTURE_TYPE_2x2_HEXAGON;

}
*/

//-------------------------------------//
/*
void clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;

	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId);

	dest->height = gfxTexture.height;
	dest->width = gfxTexture.width;
	dest->xRel = gfxTexture.xRel;
	dest->yRel = gfxTexture.yRel;
	dest->textType = TEXTURE_TYPE_PLANE;

	//- Create one OpenGL texture
	dest->texture = getInstance().getFreeTexture();

	
	//- Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dest->width, dest->height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, gfxTexture.imageRGBA);
	checkForGlError("glTexImage2D()");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);
}
*/

//-------------------------------------//
void clOpenGLTexturesHelper::loadObjectTextureFromGFX(ty_TextureObject * dest, clGFXFile * gfxFileObj, int gfxTextureId, int gfxSharowId, int gfxFram)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;

	gfxFileObj->getTextureObject(&gfxTexture, gfxTextureId, gfxSharowId, gfxFram);

	dest->height = gfxTexture.height;
	dest->width = gfxTexture.width;
	dest->xRel = gfxTexture.xRel;
	dest->yRel = gfxTexture.yRel;
	dest->textType = TEXTURE_TYPE_PLANE;

	//- Create one OpenGL texture
	glGenTextures(1, &dest->texture);

	//- "Bind" the newly created texture
	glBindTexture(GL_TEXTURE_2D, dest->texture);

	//- Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dest->width, dest->height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, gfxTexture.imageRGBA);
	checkForGlError("glTexImage2D()");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);
}


//-------------------------------------//
bool clOpenGLTexturesHelper::checkForGlError(const char * errorText)
{
	int err = glGetError();

	if (err == GL_NO_ERROR) return false;

	const char * GL_Error_text = "";
	switch (err)
	{
		case GL_INVALID_ENUM: GL_Error_text = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE: GL_Error_text = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: GL_Error_text = "GL_INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: GL_Error_text = "GL_INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: GL_Error_text = "GL_OUT_OF_MEMORY"; break;
		case GL_STACK_UNDERFLOW: GL_Error_text = "GL_STACK_UNDERFLOW"; break;
		case GL_STACK_OVERFLOW: GL_Error_text = "GL_STACK_OVERFLOW"; break;
	}

	getInstance().m_error.AddError("%s Err: %i [%s]", errorText, err, GL_Error_text);

	return true;
}


//-------------------------------------//
void clOpenGLTexturesHelper::load_game_objects(clGFXFile &gfxObjects, clOpenGLTexturesHelper::ty_TextureObject txObjects[255])
{
	//- 0: Stacks

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_1], &gfxObjects, 1, 1, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_2], &gfxObjects, 2, 2, 0);

	//- 3: ELM fall

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_1], &gfxObjects, 4, 4, 0);
	//loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_2], &gfxObjects,	renderer, 5, 5, 0); //- same as [GAME_OBJECT_TREE_ELM_2]... not used in Map

	//- 6: OAK fall

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_1], &gfxObjects, 7, 7, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_2], &gfxObjects, 8, 8, 0);

	//- 9: BIRCH fall

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_1], &gfxObjects, 10, 10, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_2], &gfxObjects, 11, 11, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_3], &gfxObjects, 13, 13, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_4], &gfxObjects, 14, 14, 0);



	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_1], &gfxObjects, 16, 16, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_2], &gfxObjects, 17, 17, 0);



	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_SMALL], &gfxObjects, 22, 22, 0);



	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_1], &gfxObjects, 27, 26, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_2], &gfxObjects, 27, 26, 1);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_3], &gfxObjects, 27, 26, 2);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_1], &gfxObjects, 27, 26, 3);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_2], &gfxObjects, 27, 26, 4);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_3], &gfxObjects, 27, 26, 5);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_1], &gfxObjects, 27, 26, 6);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_2], &gfxObjects, 27, 26, 7);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_1], &gfxObjects, 27, 26, 8);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_2], &gfxObjects, 27, 26, 9);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_1], &gfxObjects, 27, 26, 10);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_2], &gfxObjects, 27, 26, 11);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_3], &gfxObjects, 27, 26, 12);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_4], &gfxObjects, 27, 26, 13);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BONES], &gfxObjects, 27, 26, 14);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_1], &gfxObjects, 27, 26, 15);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_2], &gfxObjects, 27, 26, 16);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_3], &gfxObjects, 27, 26, 17);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_1], &gfxObjects, 27, 26, 18);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_2], &gfxObjects, 27, 26, 19);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_3], &gfxObjects, 27, 26, 20);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_4], &gfxObjects, 27, 26, 21);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_1], &gfxObjects, 27, 26, 22);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_2], &gfxObjects, 27, 26, 23);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_3], &gfxObjects, 27, 26, 24);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_4], &gfxObjects, 27, 26, 25);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_5], &gfxObjects, 27, 26, 26);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_1], &gfxObjects, 27, 26, 27);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_2], &gfxObjects, 27, 26, 28);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_3], &gfxObjects, 27, 26, 29);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_4], &gfxObjects, 27, 26, 30);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_5], &gfxObjects, 27, 26, 31);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_6], &gfxObjects, 27, 26, 32);


	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_1], &gfxObjects, 28, 27, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_2], &gfxObjects, 28, 27, 1);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_3], &gfxObjects, 28, 27, 2);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_4], &gfxObjects, 28, 27, 3);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_5], &gfxObjects, 28, 27, 4);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_GRAVE], &gfxObjects, 28, 27, 5);


	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_1], &gfxObjects, 29, 28, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_2], &gfxObjects, 29, 28, 1);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_3], &gfxObjects, 29, 28, 2);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_4], &gfxObjects, 29, 28, 3);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_5], &gfxObjects, 29, 28, 4);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_6], &gfxObjects, 29, 28, 5);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_7], &gfxObjects, 29, 28, 6);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_8], &gfxObjects, 29, 28, 7);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_1], &gfxObjects, 29, 28, 8);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_2], &gfxObjects, 29, 28, 9);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_3], &gfxObjects, 29, 28, 10);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_4], &gfxObjects, 29, 28, 11);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_1], &gfxObjects, 29, 28, 12);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_2], &gfxObjects, 29, 28, 13);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_3], &gfxObjects, 29, 28, 14);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_4], &gfxObjects, 29, 28, 15);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_5], &gfxObjects, 29, 28, 16);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_6], &gfxObjects, 29, 28, 17);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_7], &gfxObjects, 29, 28, 18);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_8], &gfxObjects, 29, 28, 19);

	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_SMALL], &gfxObjects, 29, 28, 20);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_MEDIUM], &gfxObjects, 29, 28, 21);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_LARGE], &gfxObjects, 29, 28, 22);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_XLARGE], &gfxObjects, 29, 28, 23);


	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_1], &gfxObjects, 30, 29, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_2], &gfxObjects, 30, 29, 1);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_3], &gfxObjects, 30, 29, 2);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_4], &gfxObjects, 30, 29, 3);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_5], &gfxObjects, 30, 29, 4);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_6], &gfxObjects, 30, 29, 5);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_7], &gfxObjects, 30, 29, 6);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_8], &gfxObjects, 30, 29, 7);




	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_01], &gfxObjects, 31, 30, 0);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_02], &gfxObjects, 31, 30, 1);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_03], &gfxObjects, 31, 30, 2);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_04], &gfxObjects, 31, 30, 3);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_05], &gfxObjects, 31, 30, 4);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_06], &gfxObjects, 31, 30, 5);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_07], &gfxObjects, 31, 30, 6);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_08], &gfxObjects, 31, 30, 7);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_09], &gfxObjects, 31, 30, 8);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_10], &gfxObjects, 31, 30, 9);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_11], &gfxObjects, 31, 30, 10);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_12], &gfxObjects, 31, 30, 11);
	loadObjectTextureFromGFX(&txObjects[clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_13], &gfxObjects, 31, 30, 12);
}



/*
void load_game_buildings_roman(SDL_Renderer *renderer, clGFXFile &gfxObjects, clGFXFile::GFX_ObjectTexture txObjects[255])
{

return
}
*/