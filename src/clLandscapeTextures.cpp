#include "clLandscapeTextures.h"

//-------------------------------------//
clLandscapeTextures::clLandscapeTextures(int TextureAtlasWidth, int TextureAtlasHeight, int TriangleCount)
	: clTextureAtlas(TextureAtlasWidth, TextureAtlasHeight)
{
	m_TriangleListPos = -1;
	m_TriangleListSize = TriangleCount;

	m_TriangleList = new tyTriangleTexture[m_TriangleListSize];
}

//-------------------------------------//
clLandscapeTextures::~clLandscapeTextures()
{
	delete [] m_TriangleList;
}


//-------------------------------------//
unsigned int clLandscapeTextures::getTriangleMapId(unsigned char p1, unsigned char p2, unsigned char p3)
{
	return (((p1 << 8) | p2) << 8) | p3;
}

//-------------------------------------//
int clLandscapeTextures::findTriangle(unsigned int TriangleMapId, bool returnNegativeIfNotFound)
{
	int minPos = 0;
	int maxPos = m_TriangleListPos;

	if (m_TriangleListPos < 0) return 0;

	unsigned int minVal = m_TriangleList[minPos].MapTypeId;
	unsigned int maxVal = m_TriangleList[maxPos].MapTypeId;

	if (TriangleMapId == maxVal) return maxPos;
	if (TriangleMapId == minVal) return minPos;

	if (returnNegativeIfNotFound)
	{
		if (TriangleMapId>maxVal) return -(maxPos+1);
		if (TriangleMapId<minVal) return -1;
	}
	else
	{
		if (TriangleMapId>maxVal) return maxPos + 1;
		if (TriangleMapId<minVal) return minPos;
	}

	int pos=0;
	unsigned int val=0;

	while (minPos <= maxPos)
	{
		pos = (maxPos + minPos) / 2;
		val = m_TriangleList[pos].MapTypeId;

		if (TriangleMapId > val)
		{
			minPos = pos+1;
		}
		else if (TriangleMapId < val)
		{
			maxPos = pos-1;
		}
		else //- TriangleMapId == val
		{
			return pos;
		}
	}

	if (returnNegativeIfNotFound) if (m_TriangleList[pos].MapTypeId != TriangleMapId) return -minPos;

	return minPos;
}

//-------------------------------------//
clLandscapeTextures::tyTriangleTexture * clLandscapeTextures::createNewTriangle(unsigned int TriangleMapId)
{
	if (m_TriangleListPos == m_TriangleListSize)
	{
		m_error.AddError("(createNewTriangle) Unable to add more Triangle information.");
		return NULL;
	}

	//- find the matched or the next item
	int newID = findTriangle(TriangleMapId);


	//m_error.AddDebug("newID: %i - val: %i  [-1]%i [0]%i [+1]%i [+2]%i", newID, TriangleMapId, m_TriangleList[newID - 1], m_TriangleList[newID], m_TriangleList[newID + 1], m_TriangleList[newID+2]);

	//- move all Items behind the ID one step to the end to create a free space
	for (int i = m_TriangleListPos; i >= newID; i--)
	{
		m_TriangleList[i + 1] = m_TriangleList[i];
	}

	//- increase List size
	m_TriangleListPos++;


	tyTriangleTexture * pTList = &m_TriangleList[newID];
	pTList->MapTypeId = TriangleMapId;
   
	return pTList;

}


//-------------------------------------//
GLuint clLandscapeTextures::getGLTextureId()
{
	if (m_GLTextureID != NULL) return m_GLTextureID;

	m_GLTextureID = createGLTextureAtlas();

	m_error.AddDebug("LandscapeTextures: count: %i out of: %i", m_TriangleListPos, m_TriangleListSize);

	return m_GLTextureID;
}



//-------------------------------------//
clLandscapeTextures::tyTriangleTexture * clLandscapeTextures::getTriangleTextureInformation(unsigned char n1, unsigned char n2, unsigned char n3)
{
	int textMapID = getTriangleMapId(n1, n2, n3);
	int id = findTriangle(textMapID, true);

	if (id >= 0)
	{
		if (textMapID != m_TriangleList[id].MapTypeId)
		{
			return NULL;
		}
		return &m_TriangleList[id];
	}
	else
	{
		return NULL;
	}
}
//-------------------------------------//
void clLandscapeTextures::copyTriangleInformation(tyTriangleTexture * dest, tyTriangleTexture * src)
{
	if ((dest == NULL) || (src == NULL)) return;

	//- save typID
	int tmpID = dest->MapTypeId;

	//- copy full type
	memcpy(dest, src, sizeof(dest));


	//- reset typID
	dest->MapTypeId = tmpID;
}

//-------------------------------------//
void clLandscapeTextures::AddTextureMapping(int fromMapType1, int  fromMapType2, int  fromMapType3, int  toMapType1, int  toMapType2, int  toMapType3)
{
	tyTriangleTexture * pTList;
	tyTriangleTexture * toId;

	//- 1.
	pTList = createNewTriangle(getTriangleMapId(fromMapType1, fromMapType2, fromMapType3));
	if (pTList == NULL) return;

	toId = getTriangleTextureInformation(toMapType1, toMapType2, toMapType3);
	copyTriangleInformation(pTList, toId);


	//- 2.
	pTList = createNewTriangle(getTriangleMapId(fromMapType2, fromMapType3, fromMapType1));
	if (pTList == NULL) return;

	toId = getTriangleTextureInformation(toMapType2, toMapType3, toMapType1);
	copyTriangleInformation(pTList, toId);


	//- 3.
	pTList = createNewTriangle(getTriangleMapId(fromMapType3, fromMapType1, fromMapType2));
	if (pTList == NULL) return;

	toId = getTriangleTextureInformation(toMapType3, toMapType1, toMapType2);
	copyTriangleInformation(pTList, toId);



}

//-------------------------------------//
void clLandscapeTextures::AddHexagonToTriangleInfo(clTextureAtlas::tyTextureAtlasPos * atlasPosVariante1, clTextureAtlas::tyTextureAtlasPos * atlasPosVariante2, int fromMapType, int toMapType)
{
	tyTriangleTexture * pTList;


	//--     . --- .           1    1 --- 2  
	//--    / \   / \         / \    \ B /
	//--   / A \ /   \       / A \    \ /   
	//--  : --- 2 --- :     1 --- 2    1 
	//--   \ B / \   / 
	//--    \ /   \ /
	//--     ' --- '

	pTList = createNewTriangle(getTriangleMapId(fromMapType, fromMapType, toMapType));
	if (pTList == NULL) return;
	pTList->texType = enumTextureType::TEXTURE_TYPE_SINGEL;

	//- A
	pTList->up_var1_x = atlasPosVariante1->x;
	pTList->up_var1_y = atlasPosVariante1->y + 16;

	pTList->up_var2_x = atlasPosVariante2->x;
	pTList->up_var2_y = atlasPosVariante2->y + 16;

	//- B
	pTList->down_var1_x = atlasPosVariante1->x;
	pTList->down_var1_y = atlasPosVariante1->y + 0;

	pTList->down_var2_x = atlasPosVariante2->x;
	pTList->down_var2_y = atlasPosVariante2->y + 0;



	//--     . --- .           2    2 --- 1  
	//--    / \   / \         / \    \ B /
	//--   /   \ /   \       / A \    \ /   
	//--  : --- 2 --- :     1 --- 1    1 
	//--   \   / \ B / 
	//--    \ / A \ /
	//--     ' --- '

	pTList = createNewTriangle(getTriangleMapId(toMapType, fromMapType, fromMapType));
	if (pTList == NULL) return;
	pTList->texType = enumTextureType::TEXTURE_TYPE_SINGEL;

	//- A
	pTList->up_var1_x = atlasPosVariante1->x + 8;
	pTList->up_var1_y = atlasPosVariante1->y + 0;

	pTList->up_var2_x = atlasPosVariante2->x + 8;
	pTList->up_var2_y = atlasPosVariante2->y + 0;

	//- B
	pTList->down_var1_x = atlasPosVariante1->x + 16;
	pTList->down_var1_y = atlasPosVariante1->y + 0;

	pTList->down_var2_x = atlasPosVariante2->x + 16;
	pTList->down_var2_y = atlasPosVariante2->y + 0;



	//--     . --- .           1    1 --- 1  
	//--    / \ B / \         / \    \ B /
	//--   /   \ / A \       / A \    \ /   
	//--  : --- 2 --- :     2 --- 1    2 
	//--   \   / \   / 
	//--    \ /   \ /
	//--     ' --- '

	pTList = createNewTriangle(getTriangleMapId(fromMapType, toMapType, fromMapType));
	if (pTList == NULL) return;
	pTList->texType = enumTextureType::TEXTURE_TYPE_SINGEL;

	//- A
	pTList->up_var1_x = atlasPosVariante1->x + 16;
	pTList->up_var1_y = atlasPosVariante1->y + 16;

	pTList->up_var2_x = atlasPosVariante2->x + 16;
	pTList->up_var2_y = atlasPosVariante2->y + 16;

	//- B
	pTList->down_var1_x = atlasPosVariante1->x + 8;
	pTList->down_var1_y = atlasPosVariante1->y + 16;

	pTList->down_var2_x = atlasPosVariante2->x + 8;
	pTList->down_var2_y = atlasPosVariante2->y + 16;

}

//-------------------------------------//
bool clLandscapeTextures::AddTextureHexagon(clGFXFile * gfxFileObj, int gfxTextureFromId_var1, int gfxTextureFromId_var2, int gfxTextureToId_var1, int gfxTextureToId_var2, int fromMapType, int toMapType)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;
	clTextureAtlas::tyTextureAtlasPos atlasPos1;
	clTextureAtlas::tyTextureAtlasPos atlasPos2;

	//-----------
	//- part 1 - variation 1
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureFromId_var1);
	//- copy image to tmp buffer
	AddTexture(&atlasPos1, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	//-----------
	//- part 1 - variation 2
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureFromId_var2);
	//- copy image to tmp buffer
	AddTexture(&atlasPos2, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	//-----------
	//- add triangles
	AddHexagonToTriangleInfo(&atlasPos1, &atlasPos2, fromMapType, toMapType);


	//-----------
	//- part 2 - variation 1
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureToId_var1);
	//- copy image to tmp buffer
	AddTexture(&atlasPos1, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	//-----------
	//- part 2 - variation 2
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureToId_var2);
	//- copy image to tmp buffer
	AddTexture(&atlasPos2, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	//-----------
	//- add triangles
	AddHexagonToTriangleInfo(&atlasPos1, &atlasPos2, toMapType, fromMapType);

	return true;
}


//-------------------------------------//
clLandscapeTextures::tyTriangleTexture * clLandscapeTextures::AddTexturePlainColored32x32(unsigned char r, unsigned char g, unsigned char b, int MapType)
{
	clTextureAtlas::tyTextureAtlasPos atlasPos;
	tyTriangleTexture * pTList;
	unsigned int imageRGBA[32 * 32];
	unsigned int color = (255 | r << 8 | g << 16 | b << 24);

	//- fill buffer
	for (int i = sizeof(imageRGBA) / sizeof(int) -1; i >= 0; i--) imageRGBA[i] = color;


	//- copy image to tmp buffer
	AddTexture(&atlasPos, (unsigned int *) &imageRGBA, 32, 32);



	pTList = createNewTriangle(getTriangleMapId(MapType, MapType, MapType));
	if (pTList == NULL) return NULL;
	pTList->texType = enumTextureType::TEXTURE_TYPE_SINGEL;

	//--  +- . --- . -+ 
	//--  | / \   / \ |
	//--  |/ A \ / A \|
	//--  : --- + --- : 
	//--  |\ B / \ B /|
	//--  | \ /   \ / |
	//--  +- ' --- ' -+


	//- A
	pTList->up_var1_x = atlasPos.x + 0;
	pTList->up_var1_y = atlasPos.y + 8;

	pTList->up_var2_x = atlasPos.x + 16;
	pTList->up_var2_y = atlasPos.y + 8;

	//- B
	pTList->down_var1_x = atlasPos.x + 0;
	pTList->down_var1_y = atlasPos.y + 16;

	pTList->down_var2_x = atlasPos.x + 16;
	pTList->down_var2_y = atlasPos.y + 16;


	return pTList;
}


//-------------------------------------//
bool clLandscapeTextures::AddTexturePlane32x32(clGFXFile * gfxFileObj, int gfxTextureId, int MapType)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;
	clTextureAtlas::tyTextureAtlasPos atlasPos;
	tyTriangleTexture * pTList;


	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId);
	//- copy image to tmp buffer
	AddTexture(&atlasPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	

	pTList = createNewTriangle(getTriangleMapId(MapType, MapType, MapType));
	if (pTList == NULL) return false;
	pTList->texType = enumTextureType::TEXTURE_TYPE_SINGEL;

	//--  +- . --- . -+ 
	//--  | / \   / \ |
	//--  |/ A \ / A \|
	//--  : --- + --- : 
	//--  |\ B / \ B /|
	//--  | \ /   \ / |
	//--  +- ' --- ' -+


	//- A
	pTList->up_var1_x = atlasPos.x + 0;
	pTList->up_var1_y = atlasPos.y + 8;

	pTList->up_var2_x = atlasPos.x + 16;
	pTList->up_var2_y = atlasPos.y + 8;

	//- B
	pTList->down_var1_x = atlasPos.x + 0;
	pTList->down_var1_y = atlasPos.y + 16;

	pTList->down_var2_x = atlasPos.x + 16;
	pTList->down_var2_y = atlasPos.y + 16;


	return true;
}

//-------------------------------------//
bool clLandscapeTextures::AddTexturePlane128x128(clGFXFile * gfxFileObj, int gfxTextureId, int MapType)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;
	clTextureAtlas::tyTextureAtlasPos atlasPos;
	tyTriangleTexture * pTList;


	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId);
	

	//- to be able to use the left most cuttet edge triangle (x) we copy 8 pixle to the end of the texture
	//--  <------ 128 -----><-8>
	//--  +-----+-----+-----+--+
	//--  |\ B / \   / \   /|\ |
	//--  |?\ / A \ /   \ / | \|
	//--  |??' --- ' --- ' -|- |
	//--  |                 |  |

	
	//- copy image to tmp buffer
	AddTexture(&atlasPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, 8);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	pTList = createNewTriangle(getTriangleMapId(MapType, MapType, MapType));
	pTList->texType = enumTextureType::TEXTURE_TYPE_REPEAT128x128;

	//- A
	pTList->up_var1_x = atlasPos.x + 0;
	pTList->up_var1_y = atlasPos.y + 0;

	pTList->up_var2_x = atlasPos.x + 0;
	pTList->up_var2_y = atlasPos.y + 0;

	//- B
	pTList->down_var1_x = atlasPos.x + 0;
	pTList->down_var1_y = atlasPos.y + 0;

	pTList->down_var2_x = atlasPos.x + 0;
	pTList->down_var2_y = atlasPos.y + 0;


	return true;
}