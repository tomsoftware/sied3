#include "clLandscapeTextures.h"

//-------------------------------------//
clLandscapeTextures::clLandscapeTextures(int TextureAtlasWidth, int TextureAtlasHeight, int TriangleCount)
	: clTextureAtlas(TextureAtlasWidth, TextureAtlasHeight)
{
	m_TriangelListPos = -1;
	m_TriangelListSize = TriangleCount;

	m_TriangelList = new tyTriangelTexture[m_TriangelListSize];
}

//-------------------------------------//
clLandscapeTextures::~clLandscapeTextures()
{
	delete [] m_TriangelList;
}


//-------------------------------------//
unsigned int clLandscapeTextures::getTriangleMapId(unsigned char p1, unsigned char p2, unsigned char p3)
{
	return (((p1 << 8) | p2) << 8) | p3;
}

//-------------------------------------//
int clLandscapeTextures::findTriangel(unsigned int TriangleMapId, bool returnNegativeIfNotFound)
{
	int minPos = 0;
	int maxPos = m_TriangelListPos;

	if (m_TriangelListPos < 0) return 0;

	unsigned int minVal = m_TriangelList[minPos].MapTypeId;
	unsigned int maxVal = m_TriangelList[maxPos].MapTypeId;

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
		val = m_TriangelList[pos].MapTypeId;

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

	if (returnNegativeIfNotFound) if (m_TriangelList[pos].MapTypeId != TriangleMapId) pos = -minPos;

	return minPos;
}

//-------------------------------------//
clLandscapeTextures::tyTriangelTexture * clLandscapeTextures::createNewTriangel(unsigned int TriangleMapId)
{
	if (m_TriangelListPos == m_TriangelListSize)
	{
		m_error.AddError("(createNewTriangel) Unable to add more triangel information.");
		return NULL;
	}

	//- find the matched or the next item
	int newID = findTriangel(TriangleMapId);


	//m_error.AddDebug("newID: %i - val: %i  [-1]%i [0]%i [+1]%i [+2]%i", newID, TriangleMapId, m_TriangelList[newID - 1], m_TriangelList[newID], m_TriangelList[newID + 1], m_TriangelList[newID+2]);

	//- move all Items behind the ID one step to the end to create a free space
	for (int i = m_TriangelListPos; i >= newID; i--)
	{
		m_TriangelList[i + 1] = m_TriangelList[i];
	}

	//- increase List size
	m_TriangelListPos++;


	tyTriangelTexture * pTList = &m_TriangelList[newID];
	pTList->MapTypeId = TriangleMapId;
   
	return pTList;

}


//-------------------------------------//
GLuint clLandscapeTextures::getGLTextureId()
{
	if (m_GLTextureID != NULL) return m_GLTextureID;

	m_GLTextureID = createGLTextureAtlas();

	m_error.AddDebug("LandscapeTextures: count: %i out of: %i", m_TriangelListPos, m_TriangelListSize);

	return m_GLTextureID;
}



//-------------------------------------//
clLandscapeTextures::tyTriangelTexture * clLandscapeTextures::getTriangelTextureInformation(unsigned char n1, unsigned char n2, unsigned char n3)
{
	int id = findTriangel(getTriangleMapId(n1, n2, n3), true);

	if (id >= 0)
	{
		return &m_TriangelList[id];
	}
	else
	{
		return NULL;
	}
}


//-------------------------------------//
void clLandscapeTextures::AddHexagonToTriangleInfo(clTextureAtlas::tyTextureAtlasPos * atlasPosVariante1, clTextureAtlas::tyTextureAtlasPos * atlasPosVariante2, int fromMapType, int toMapType)
{
	tyTriangelTexture * pTList;


	//--     . --- .           1    1 --- 2  
	//--    / \   / \         / \    \ B /
	//--   / A \ /   \       / A \    \ /   
	//--  : --- 2 --- :     1 --- 2    1 
	//--   \ B / \   / 
	//--    \ /   \ /
	//--     ' --- '

	pTList = createNewTriangel(getTriangleMapId(fromMapType, fromMapType, toMapType));
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

	pTList = createNewTriangel(getTriangleMapId(toMapType, fromMapType, fromMapType));
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

	pTList = createNewTriangel(getTriangleMapId(fromMapType, toMapType, fromMapType));
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
	//- add triangels
	AddHexagonToTriangleInfo(&atlasPos1, &atlasPos2, toMapType, fromMapType);


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
	//- add triangels
	AddHexagonToTriangleInfo(&atlasPos1, &atlasPos2, fromMapType, toMapType);

	return true;
}



//-------------------------------------//
bool clLandscapeTextures::AddTexturePlane32x32(clGFXFile * gfxFileObj, int gfxTextureId, int MapType)
{
	clGFXFile::GFXFILE_TextureObject gfxTexture;
	clTextureAtlas::tyTextureAtlasPos atlasPos;
	tyTriangelTexture * pTList;


	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureId);
	//- copy image to tmp buffer
	AddTexture(&atlasPos, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	

	pTList = createNewTriangel(getTriangleMapId(MapType, MapType, MapType));
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
	tyTriangelTexture * pTList;


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


	pTList = createNewTriangel(getTriangleMapId(MapType, MapType, MapType));
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