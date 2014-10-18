#ifndef CLLANDSCAPE_TEXTURE_H
#define CLLANDSCAPE_TEXTURE_H


#include "clError.h"
#include "clGFXFile.h"
#include "clTextureAtlas.h"

class clLandscapeTextures :public clTextureAtlas
{
public:
	clLandscapeTextures(int TextureAtlasWidth, int TextureAtlasHeight, int TriangleCount);
	~clLandscapeTextures();

	/*
	struct tyMapTypeParts
	{
		unsigned char P1;
		unsigned char P2;
		unsigned char P3;
	};

	union tyMapType
	{
		unsigned int MapType;
		tyMapTypeParts MapTypeParts;
	};
	*/



	enum enumTextureType
	{
		TEXTURE_TYPE_NOT_FOUND = 0,
		TEXTURE_TYPE_SINGEL = 1,
		TEXTURE_TYPE_REPEAT128x128 = 2,
	};

	struct tyTriangleTexture
	{
		//- Triangle up
		//-         /\ 
		//-        /  \ 
		//-  (0,0)x____\ 
		int up_var1_x; //- variation 1
		int up_var1_y;
		int up_var2_x; //- variation 2
		int up_var2_y;

		//- Triangle down
		//-       ______
		//-       \    /
		//-        \  / 
		//-  (0,0)x \/ 
		int down_var1_x; //- variation 1
		int down_var1_y;
		int down_var2_x; //- variation 2
		int down_var2_y;

		enumTextureType texType = TEXTURE_TYPE_NOT_FOUND;
		int MapTypeId=0;
	};

	tyTriangleTexture * m_TriangleList;
	int m_TriangleListPos;
	int m_TriangleListSize;

	bool AddTextureHexagon(clGFXFile * gfxFileObj, int gfxTextureFromId_var1, int gfxTextureFromId_var2, int gfxTextureToId_var1, int gfxTextureToId_var2, int fromMapType, int toMapType);
	unsigned int getTriangleMapId(unsigned char p1, unsigned char p2, unsigned char p3);
	bool AddTexturePlane32x32(clGFXFile * gfxFileObj, int gfxTextureId, int MapType);
	bool AddTexturePlane128x128(clGFXFile * gfxFileObj, int gfxTextureId, int MapType);
	clLandscapeTextures::tyTriangleTexture * AddTexturePlainColored32x32(unsigned char r, unsigned char g, unsigned char b, int MapType);

	void AddTextureMapping(int fromMapType1, int  fromMapType2, int  fromMapType3, int  toMapType1, int  toMapType2, int  toMapType3);

	GLuint getGLTextureId();
	GLuint m_GLTextureID = NULL;

	int height();
	int width();

	//- add triangels
	void AddHexagonToTriangleInfo(clTextureAtlas::tyTextureAtlasPos * atlasPosVariante1, clTextureAtlas::tyTextureAtlasPos * atlasPosVariante2, int fromMapType, int toMapType);
	
	tyTriangleTexture * getTriangleTextureInformation(unsigned char n1, unsigned char n2, unsigned char n3);


	int findTriangle(unsigned int TriangleMapId, bool returnNegativeIfNotFound = false);
private:
	tyTriangleTexture * createNewTriangle(unsigned int TriangleMapId);
	clError m_error = clError("clLandTex");
	void copyTriangleInformation(tyTriangleTexture * dest, tyTriangleTexture * src);

};


#endif
