#include "clLandscapeTextures.h"

//-------------------------------------//
clLandscapeTextures::clLandscapeTextures(int TextureAtlasWidth, int TextureAtlasHeight, int TriangleCount)
: clTextureAtlas(TextureAtlasWidth, TextureAtlasHeight)
{
	m_TriangleListPos = -1;
	m_TriangleSortListPos = -1;

	m_TriangleListSize = TriangleCount;

	m_TriangleList = new tyTriangleTexture[m_TriangleListSize];
	m_TriangleListSortList = new int[m_TriangleListSize];
}

//-------------------------------------//
clLandscapeTextures::~clLandscapeTextures()
{
	delete [] m_TriangleList;
	delete [] m_TriangleListSortList;
}


/// <summary>
/// returns the unique ID for a triangle with the given corner values [p1], [p2], [p3]
/// --   Type A      Type B
/// --      1       1 --- 3
/// --     / \       \ B / 
/// --    / A \       \ /  
/// --   2 --- 3       2
/// </summary>
unsigned int clLandscapeTextures::getTriangleMapId(unsigned char p1, unsigned char p2, unsigned char p3)
{
	return (((p1 << 8) | p2) << 8) | p3;
}


/// <summary>
/// Find the triangle-pos in [m_TriangleList].
/// <return>&gt;0 on found; 0 on error; &lt;0 on not found: correct position is ABS(return)</return>
/// </summary>
int clLandscapeTextures::findTriangle(unsigned int TriangleMapId, bool returnNegativeIfNotFound)
{
	int minPos = 0;
	int maxPos = m_TriangleSortListPos;

	if (m_TriangleSortListPos < 0) return 0;

	unsigned int minVal = m_TriangleList[m_TriangleListSortList[minPos]].MapTypeId;
	unsigned int maxVal = m_TriangleList[m_TriangleListSortList[maxPos]].MapTypeId;

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
		val = m_TriangleList[m_TriangleListSortList[pos]].MapTypeId;

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

	if (returnNegativeIfNotFound) if (m_TriangleList[m_TriangleListSortList[pos]].MapTypeId != TriangleMapId) return -minPos;

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
	int newSortID = findTriangle(TriangleMapId);


	//m_error.AddDebug("newID: %i - val: %i  [-1]%i [0]%i [+1]%i [+2]%i", newID, TriangleMapId, m_TriangleList[newID - 1], m_TriangleList[newID], m_TriangleList[newID + 1], m_TriangleList[newID+2]);

	//- move all index Items behind the ID one step to the end to create a free space
	for (int i = m_TriangleSortListPos; i >= newSortID; i--)
	{
		m_TriangleListSortList[i + 1] = m_TriangleListSortList[i];
	}

	//- increase List size (starts with -1)
	m_TriangleListPos++;
	m_TriangleSortListPos++;

	int newListPos = m_TriangleListPos;

	m_TriangleListSortList[newSortID] = newListPos;

	//- fill new Item
	tyTriangleTexture * pTList = &m_TriangleList[newListPos];
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
clLandscapeTextures::tyTriangleTexture * clLandscapeTextures::getTriangleTextureInformation(int index)
{
	return &m_TriangleList[index];
}


//-------------------------------------//
clLandscapeTextures::tyTriangleTexture * clLandscapeTextures::findTriangleTextureInformation(unsigned char n1, unsigned char n2, unsigned char n3)
{
	int textMapID = getTriangleMapId(n1, n2, n3);
	int sortId = findTriangle(textMapID, true);

	if (sortId >= 0)
	{
		int id = m_TriangleListSortList[sortId];

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
int clLandscapeTextures::getTriangleTextureID(unsigned char n1, unsigned char n2, unsigned char n3)
{
	int textMapID = getTriangleMapId(n1, n2, n3);
	int sortId = findTriangle(textMapID, true);

	if (sortId < 0) return -1;

	return m_TriangleListSortList[sortId];

}

//-------------------------------------//
int clLandscapeTextures::getTriangleTextureIDCount()
{
	return m_TriangleListPos;
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

	toId = findTriangleTextureInformation(toMapType1, toMapType2, toMapType3);
	copyTriangleInformation(pTList, toId);


	//- 2.
	pTList = createNewTriangle(getTriangleMapId(fromMapType2, fromMapType3, fromMapType1));
	if (pTList == NULL) return;

	toId = findTriangleTextureInformation(toMapType2, toMapType3, toMapType1);
	copyTriangleInformation(pTList, toId);


	//- 3.
	pTList = createNewTriangle(getTriangleMapId(fromMapType3, fromMapType1, fromMapType2));
	if (pTList == NULL) return;

	toId = findTriangleTextureInformation(toMapType3, toMapType1, toMapType2);
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
	//- fix texture-background
	fixTextureBackground(gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, LANDSCAPE_MASK_COLOR1, LANDSCAPE_MASK_COLOR2);
	//- copy image to tmp buffer
	AddTexture(&atlasPos1, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);

	//-----------
	//- part 1 - variation 2
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureFromId_var2);
	//- fix texture-background
	fixTextureBackground(gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, LANDSCAPE_MASK_COLOR1, LANDSCAPE_MASK_COLOR2);
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
	//- fix texture-background
	fixTextureBackground(gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, LANDSCAPE_MASK_COLOR1, LANDSCAPE_MASK_COLOR2);
	//- copy image to tmp buffer
	AddTexture(&atlasPos1, gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height);
	//- unload image data
	gfxFileObj->unload_GFX_Object(&gfxTexture);


	//-----------
	//- part 2 - variation 2
	gfxFileObj->getTextureLandscape(&gfxTexture, gfxTextureToId_var2);
	//- fix texture-background
	fixTextureBackground(gfxTexture.imageRGBA, gfxTexture.width, gfxTexture.height, LANDSCAPE_MASK_COLOR1, LANDSCAPE_MASK_COLOR2);
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
int clLandscapeTextures::AddTexturePlainColored32x32(unsigned char r, unsigned char g, unsigned char b)
{
	clTextureAtlas::tyTextureAtlasPos atlasPos;
	unsigned int imageRGBA[32 * 32];
	unsigned int color = (255 | b << 8 | g << 16 | r << 24);

	//- fill buffer
	for (int i = sizeof(imageRGBA) / sizeof(int) -1; i >= 0; i--) imageRGBA[i] = color;


	//- copy image to tmp buffer
	AddTexture(&atlasPos, (unsigned int *) &imageRGBA, 32, 32);



	//- increase List size (starts with -1)
	m_TriangleListPos++;

	int newListPos = m_TriangleListPos;

	//- fill new Item
	tyTriangleTexture * pTList = &m_TriangleList[newListPos];


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


	return newListPos;
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
	

	//- to be able to use the left most cuttet edge triangle (x) we copy 8 pixel to the end of the texture
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


/// <summary>
/// to avoid Texture-mapping problems on the edges with open-gl we replace the background [maskColor1] and [maskColor2] of the image.
///     .-----.           ..-----....
///    /       \        ///       \\\
///   /         \       //         \\
///  :           :  to  :           :
///   \         /       \\         //
///    \       /        \\\       ///
///     '-----'         ''''-----''''
/// </summary>
void clLandscapeTextures::fixTextureBackground(unsigned int * data, int width, int height, unsigned int maskColor1, unsigned int maskColor2)
{
	int pointCount = width * height;
	unsigned int * in = data + 1; //- start at second pixel

	//- this is not the correct way cause pixel on the left will set to the value of the last color of previous line
	//- but we dont care about the right value!

	for (int i = pointCount - 2; i > 0; i--)
	{
		if ((*in == maskColor1) || (*in == maskColor2))
		{
			if ((*(in + 1) != maskColor1) && (*(in + 1) != maskColor2))
			{
				*in = *(in + 1); //- use the color of the next point
			}
			else
			{
				*in = *(in - 1); //- use the color of the previous point
			}
		}

		in++; //- next point
	}
}