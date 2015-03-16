#include "clObjectTextures.h"


//-------------------------------------//
clObjectTextures::clObjectTextures(int TextureAtlasWidth, int TextureAtlasHeight, int ObjectCount)
	: clTextureAtlas(TextureAtlasWidth, TextureAtlasHeight)
{
	m_Textures = new ty_Texture[ObjectCount];
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
clObjectTextures::ty_TextureObject * clObjectTextures::getObjectTextures()
{
	return m_TextureObject;
}

//-------------------------------------//
clObjectTextures::ty_Texture * clObjectTextures::getTextures()
{
	return m_Textures;
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
bool clObjectTextures::createTextureInformation(int gameObjectID, int FrameCount)
{
	if ((gameObjectID < 0) || (gameObjectID > 255))
	{
		m_error.AddError("AddTexturePlaneSequenz() gameObjectID is out of range: %i", gameObjectID);
		return false;
	}


	m_TextureObject[gameObjectID].firstTexture = m_TexturesPos;
	m_TextureObject[gameObjectID].frameCount = FrameCount;


	for (int frame = 0; frame < FrameCount; frame++)
	{
		if (m_TexturesPos >= m_TexturesCount)
		{
			m_error.AddError("AddTexturePlaneSequenz() Out of Texture-Objects-Space");
			return false;
		}

		//- use next slot to store texture
		ty_Texture * dest = &m_Textures[m_TexturesPos];

		//- reset some
		memset(dest, 0, sizeof(ty_Texture));

		//- move to next free item
		m_TexturesPos++;
	}
}


//-------------------------------------//
int clObjectTextures::setTextureInformation(int gameObjectID, int SequenzIndex, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId, int FrameIndex)
{
	if ((gameObjectID < 0) || (gameObjectID > 255))
	{
		m_error.AddError("setTextureInformation() gameObjectID is out of range: %i", gameObjectID);
		return -1;
	}
	if ((SequenzIndex < 0) || (SequenzIndex >= m_TextureObject[gameObjectID].frameCount))
	{
		m_error.AddError("setTextureInformation() SequenzIndex is out of range: %i", SequenzIndex);
		return -1;
	}


	clGFXFile::GFXFILE_TextureObject gfxTexture;
	tyTextureAtlasPos texDestPos;

	//- size of Texture Atlas to calc GL texture position [0..1]
	float textureAtlasHeight = (float) getHeight();
	float textureAtlasWidth = (float) getWidth();

	//- use [TextureIndex] slot in [gameObjectID] to store texture
	int TextureIndex = m_TextureObject[gameObjectID].firstTexture + SequenzIndex;
	ty_Texture * dest = &m_Textures[TextureIndex];


	//- read gfx image
	gfxFileObj->getTextureObject(&gfxTexture, gfxTextureId, gfxShadowId, FrameIndex);

	dest->height = gfxTexture.height;
	dest->width = gfxTexture.width;
	dest->xRel = gfxTexture.xRel;
	dest->yRel = gfxTexture.yRel;

	//- store gfx image
	AddTextureToAtlas(&texDestPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, 0);

	dest->texture_x = texDestPos.x / textureAtlasWidth;
	dest->texture_y = texDestPos.y / textureAtlasHeight;
	dest->texture_r = (texDestPos.width + texDestPos.x) / textureAtlasWidth;
	dest->texture_b = (texDestPos.height + texDestPos.y) / textureAtlasHeight;


	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	return TextureIndex;

}


//-------------------------------------//
bool clObjectTextures::AddTexturePlane(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId, int FrameIndex)
{
	//- reserve Texture-information space
	if (!createTextureInformation(gameObjectID, 1)) return false;

	//- Add Texture-information to index=0
	setTextureInformation(gameObjectID, 0, gfxFileObj, gfxTextureId, gfxShadowId, FrameIndex);

	return true;
}


//-------------------------------------//
bool clObjectTextures::AddTexturePlaneSequenz(int gameObjectID, clGFXFile * gfxFileObj, int gfxTextureId, int gfxShadowId)
{
	//- frames count 
	int count = gfxFileObj->getTextureObjectSequenzCount(gfxTextureId);


	//- reserve Texture-information space
	if (!createTextureInformation(gameObjectID, count)) return false;

	//- Add Texture-information
	for (int frame = 0; frame < count; frame++)
	{
		setTextureInformation(gameObjectID, frame, gfxFileObj, gfxTextureId, gfxShadowId, frame);
	}

	return true;

}