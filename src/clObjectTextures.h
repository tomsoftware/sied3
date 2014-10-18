#ifndef CLOBJECT_TEXTURE_H
#define CLOBJECT_TEXTURE_H

#include "clError.h"
#include "clGFXFile.h"
#include "clTextureAtlas.h"


class clObjectTextures :public clTextureAtlas
{

public:
	struct ty_Objects
	{
		float texture_x; //- texture pos x
		float texture_y;
		float texture_r; //- texture right
		float texture_b; //- texture button

		int xRel = 0;
		int yRel = 0;
		int width = 0;
		int height = 0;
	};


	struct ty_TextureObject
	{
		ty_Objects * firstTexture;
		int frameCount = 0;
	};


private:
	clError m_error = clError("clObjTex");





	ty_TextureObject m_TextureObject[256];

	ty_Objects * m_Textures;
	int m_TexturesCount = 0;
	int m_TexturesPos = 0;

public:
	clObjectTextures(int TextureAtlasWidth, int TextureAtlasHeight, int ObjectCount);
	~clObjectTextures();

	ty_TextureObject * getObjectTexturs();

	GLuint getGLTextureId();
	GLuint m_GLTextureID = NULL;

	bool AddTexturePlaneSequenz(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId);
	bool AddTexturePlane(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId, int );
};

#endif