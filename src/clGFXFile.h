#ifndef CLGFXFILE_H
#define CLGFXFILE_H

#include "clError.h"
#include "clFile.h"
#include "clConfig.h"


class clGFXFile
{
public:

	struct GFXFILE_TextureObject
	{
		unsigned int *imageRGBA=NULL;
		int xRel=0;
		int yRel=0;
		int width=0;
		int height=0;
	};



	struct GFX_ObjectAnimationFrame
	{
		short posX;
		short posY;
		//unsigned short flags;
		unsigned short object_file;
		unsigned short object_id;
		unsigned short torso_id;
		unsigned short torso_file;
		unsigned short shadow_id;
		unsigned short shadow_file;
		unsigned short object_frame;
		unsigned short torso_frame;
		short sound_flag1;
		short sound_flag2;
	};



	enum enum_GFX_Type
	{
		GFX_Type_none = 0,
		GFX_Type_Landscape = 1,
		GFX_Type_Gui = 2,
		GFX_Type_Object = 3,
		GFX_Type_Torso = 4,
		GFX_Type_Shadow = 5,
		GFX_Type_AnimationInfo = 6,
		GFX_Type_Pallet = 7,
		GFX_Type____COUNT,
	};

	enum enum_COLOR_Type
	{
		COLOR_BIT_DEPTH_555 = 0x7c,
		COLOR_BIT_DEPTH_565 = 0xf8
	};

	static const char * const GFX_TYPE_NAMES[];


private:
	class clFileReader * m_FR;
	clFile m_F = clFile();
	const char * m_filename;
	clError m_error = clError("clGFX");

	enum_COLOR_Type m_color_bit_depth = enum_COLOR_Type::COLOR_BIT_DEPTH_555;

	struct ty_gfxSequenz
	{
		int HaedOffset;
		int imgCount;
		int * imgOffset;
	};

	struct ty_gfxObjects
	{
		ty_gfxSequenz *objects=NULL;
		int offset=0;
		int count=0;
		int word1;
		int word2;
		int HeaderSize;
		int word3;
		int word4;

	};

	ty_gfxObjects m_gfxObjects[GFX_Type____COUNT];

	struct ty_ImageHead
	{
		int width;
		int height;
		int DWord1;
		int xRel;
		int yRel;
		int colorMode;
		int fileOffset;
	};

	bool readImageHeader(ty_ImageHead * imgHead, int offset, bool hasMagic = false, bool hasRelatives = false, bool hasColorMode=true);
	//bool readImageData(int * outImgData, int fileOffset, int width, int height);
	bool readImageData(unsigned int * outImgData, int fileOffset, int width, int height, int x0, int y0, int frameWidth);
	bool readShadowData(unsigned int * outImgData, int fileOffset, int width, int height, int x0, int y0, int frameWidth, bool doEdgeBlure = true);
	bool readImageDataClear(unsigned int * outImgData, int width, int height);
	bool readIndexData(unsigned char * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth);

	void cleanUp();

	int readSequenz(enum_GFX_Type GFX_Type, int id);
public:
	clGFXFile(const char * fileName=NULL);
	~clGFXFile();

	bool openGFXFile(const char * fileName);

	bool getTextureLandscape(GFXFILE_TextureObject *outGFXObject, int id);
	
	bool getTextureObject(GFXFILE_TextureObject *outGFXObject, int sequenzeId, int shadowId, int frame = 0);
	bool getTextureTorso(GFXFILE_TextureObject * outGFXObject, int id, int frame);

	int getTextureLandscapeCount();
	int getTextureObjectCount();
	int getTextureObjectSequenzCount(int sequenzeId);

	bool getAnimationInfo(GFX_ObjectAnimationFrame * outGFXObject, int id, int frame);
	int getAnimationInfoFrameCount(int id);

	//SDL_Palette* getPalette(int paletteId, int colorVariationIndex);
	static void unload_GFX_Object(GFXFILE_TextureObject *outGFXObject);
	
};


__declspec(selectany)
const char * const clGFXFile::GFX_TYPE_NAMES[] = { "", "Landscape", "Gui", "Objects", "Torso", "Shadow", "AnimationInfo", "Pallet", "" };


#endif