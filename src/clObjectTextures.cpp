#include "clObjectTextures.h"


//-------------------------------------//
clObjectTextures::clObjectTextures(int TextureAtlasWidth, int TextureAtlasHeight, int ObjectCount)
	: clTextureAtlas(TextureAtlasWidth, TextureAtlasHeight)
{
	m_Textures = new ty_Objects[ObjectCount];
	m_TexturesCount = ObjectCount;
	m_TexturesPos = 0;

	for (int i = 0; i < 256; i++) m_TextureObject[i].frameCount = 0;
}


//-------------------------------------//
clObjectTextures::~clObjectTextures()
{
	delete []m_Textures;
	for (int i = 0; i < 256; i++) m_TextureObject[i].frameCount = 0;

}



//-------------------------------------//
clObjectTextures::ty_TextureObject * clObjectTextures::getObjectTexturs()
{
	return m_TextureObject;
}


//-------------------------------------//
GLuint clObjectTextures::getGLTextureId()
{
	if (m_GLTextureID != NULL) return m_GLTextureID;

	m_GLTextureID = createGLTextureAtlas();

	m_error.AddDebug("ObjectTextures: Object count: %i", m_TexturesPos);

	return m_GLTextureID;
}


//-------------------------------------//
bool clObjectTextures::AddTexturePlane(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId, int FrameIndex)
{
	if ((gameObjectID < 0) || (gameObjectID > 255))
	{
		m_error.AddError("AddTexturePlane() gameObjectID is out of range: %i", gameObjectID);
		return false;
	}


	clGFXFile::GFXFILE_TextureObject gfxTexture;
	tyTextureAtlasPos texDestPos;

	//- size of Texture Atlas to calc GL texture position [0..1]
	float textureAtlasHeight = (float)getHeight();
	float textureAtlasWidth = (float)getWidth();


	if (m_TexturesPos >= m_TexturesCount)
	{
		m_error.AddError("AddTexturePlane() Out of Texture-Objects-Space");
		return false;
	}

	//- use next slot to store texture
	ty_Objects * dest = &m_Textures[m_TexturesPos];


	//- this is the first Item
	m_TextureObject[gameObjectID].firstTexture = dest;
	m_TextureObject[gameObjectID].frameCount = 1;


	//- move to next free item
	m_TexturesPos++;


	//- read gfx image
	gfxFileObj->getTextureObject(&gfxTexture, gfxTextureId, gfxShadowId, FrameIndex);

	dest->height = gfxTexture.height;
	dest->width = gfxTexture.width;
	dest->xRel = gfxTexture.xRel;
	dest->yRel = gfxTexture.yRel;

	//- store gfx image
	AddTexture(&texDestPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, 0);

	dest->texture_x = texDestPos.x / textureAtlasWidth;
	dest->texture_y = texDestPos.y / textureAtlasHeight;
	dest->texture_r = (texDestPos.width + texDestPos.x) / textureAtlasWidth;
	dest->texture_b = (texDestPos.height + texDestPos.y) / textureAtlasHeight;


	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	return true;
}

//-------------------------------------//
bool clObjectTextures::AddTexturePlaneSequenz(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId)
{
	if ((gameObjectID < 0) || (gameObjectID > 255))
	{
		m_error.AddError("AddTexturePlaneSequenz() gameObjectID is out of range: %i", gameObjectID);
		return false;
	}

	//- frames count 
	int count = gfxFileObj->getTextureObjectSequenzCount(gfxTextureId);


	clGFXFile::GFXFILE_TextureObject gfxTexture;
	tyTextureAtlasPos texDestPos;

	//- size of Texture Atlas to calc GL texture position. Range: [0..1]
	float textureAtlasHeight = (float)getHeight();
	float textureAtlasWidth = (float)getWidth();

	for (int frame = 0; frame < count; frame++)
	{
		if (m_TexturesPos >= m_TexturesCount)
		{
			m_error.AddError("AddTexturePlaneSequenz() Out of Texture-Objects-Space");
			return false;
		}

		//- use next slot to store texture
		ty_Objects * dest = &m_Textures[m_TexturesPos];

		if(frame == 0)
		{
			//- this is the first item
			m_TextureObject[gameObjectID].firstTexture = dest;
			m_TextureObject[gameObjectID].frameCount = count;

		}

		//- move to next free item
		m_TexturesPos++;


		//- read gfx image
		gfxFileObj->getTextureObject(&gfxTexture, gfxTextureId, gfxShadowId, frame);

		dest->height = gfxTexture.height;
		dest->width = gfxTexture.width;
		dest->xRel = gfxTexture.xRel;
		dest->yRel = gfxTexture.yRel;

		//- store gfx image
		AddTexture(&texDestPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, 0);

		dest->texture_x = texDestPos.x / textureAtlasWidth;
		dest->texture_y = texDestPos.y / textureAtlasHeight;
		dest->texture_r = (texDestPos.width + texDestPos.x) / textureAtlasWidth;
		dest->texture_b = (texDestPos.height + texDestPos.y) / textureAtlasHeight;


		//- unload image data
		gfxFileObj->unload_GFX_Object(&gfxTexture);

	}

	return true;
	

}