#ifndef CLMAPFILEREADER_H
#define CLMAPFILEREADER_H

#include "clError.h"
#include "clFile.h"
#include "clConfig.h"
#include <SDL.h>

class clMapFileReader
{

public:
	enum enum_map_folders
	{
		FOLDER_MULTI,
		FOLDER_SINGLE,
		FOLDER_TUTORIAL,
		FOLDER_USER
	};

	enum enum_map_file_parts
	{
		PART_TYPE_EOF = 0, // End of File and Padding
		PART_TYPE_MapInfo = 1,
		PART_TYPE_PlayerInfo = 2,
		PART_TYPE_TeamInfo = 3,
		PART_TYPE_Preview = 4,
		PART_TYPE_Area = 6,
		PART_TYPE_Settlers = 7,
		PART_TYPE_Buildings = 8,
		PART_TYPE_Resources = 9,
		PART_TYPE_QuestText = 11,
		PART_TYPE_QuestTip = 12,
		PART_TYPE____COUNT
	};

	enum enum_map_file_version
	{
		FILE_VERSION_NO_S3_FILE = 0x00,
		FILE_VERSION_DEFAULT = 0x0A,
		FILE_VERSION_AMAZONS = 0x0B
	};

private:

	enum_map_file_version m_fileVersion = FILE_VERSION_NO_S3_FILE;

	struct ty_file_part
	{
		int offset=0;
		int size=0;
		int cryptKey=0;
		bool hasBeenDecrypted = false;
	};

	enum enum_NATION
	{
		NATION_Romans = 0,
		NATION_Egyptians = 1,
		NATION_Asians = 2,
		NATION_Amazons = 3,
		NATION_FreeChoice = 255,
		NATION_NOT_DEFINED = 256
	};

	struct ty_player
	{
		int startX;
		int startY;
		char * PName = "";
		enum_NATION Nation;
	};

	enum enum_STARTRESOURCES
	{
		STARTRESOURCES_less = 1,
		STARTRESOURCES_medium = 2,
		STARTRESOURCES_much = 3
	};

	//----------//
	int m_MapSizeHeight = 0;
	int m_MapSizeWidth = 0;
	int m_PlayerCount = 0;
	bool m_isSinglePlayerMap = true;
	enum_STARTRESOURCES m_startResources;
	ty_player m_player[20];
	//----------//
	 

	ty_file_part m_fileParts[enum_map_file_parts::PART_TYPE____COUNT];
	bool doUncrypt(clFileReader * FR, ty_file_part *FPart);

	clError m_error = clError("clMapFileReader");
	clFile m_File;

	const char * getMapPartTypeName(enum_map_file_parts partType);

	bool checkChecksum(clFileReader * FR);
	bool loadMap(clFileReader * FR, enum_map_file_version fileVersion);

	bool readMapInfo();



public:
	clMapFileReader(enum_map_folders map_folder, const char * fileName);
	SDL_Texture * getPreviewImage(SDL_Renderer* renderer, int shearFaktor=0);
	~clMapFileReader();
	int getMapWidth() { return m_MapSizeWidth; };
	int getMapHeight() { return m_MapSizeHeight; };

	bool readMapArea(unsigned int *outBuffer_AraeHeightObjects = NULL, int sizeOfBuffer_AraeHeightObject = 0, unsigned int *outBuffer_PlayerAccessibleResources = NULL, int sizeOfBuffer_PlayerAccessibleResources = 0);
};

#endif