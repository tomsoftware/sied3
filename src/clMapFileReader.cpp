#include "clMapFileReader.h"


//- scale Color from RGB555 to RGB888
//- 255 / 248 = 1.02822580645
//#define RGB555_TO_RGB888_FAKTOR 1.02822580645
#define RGB555_TO_RGB888_FAKTOR 1.03

clMapFileReader::clMapFileReader(enum_map_folders map_folder, const char * fileName)
{
	std::string tmpFileName;
	
	char * folderName="";

	switch (map_folder)
	{
		case clMapFileReader::FOLDER_MULTI:		tmpFileName.append("MULTI\\");	break;
		case clMapFileReader::FOLDER_SINGLE:	tmpFileName.append("SINGLE\\");	break;
		case clMapFileReader::FOLDER_TUTORIAL:	tmpFileName.append("TUTORIAL\\");	break;
		case clMapFileReader::FOLDER_USER:		tmpFileName.append("USER\\");	break;
	}

	tmpFileName.append(fileName);
	

	m_File.readFromFile(clConfig::getPath(clConfig::enum_config_value::CONFIG_Map_Path), tmpFileName.c_str());
	clFileReader FR = clFileReader(&m_File);


	if (!checkChecksum(&FR))
	{
		m_error.AddError("Wrong checksum of map-file %s", tmpFileName.c_str());
		return;
	}


	m_fileVersion = (enum_map_file_version)FR.readIntBE();

	if ((m_fileVersion != enum_map_file_version::FILE_VERSION_DEFAULT) && (m_fileVersion != enum_map_file_version::FILE_VERSION_AMAZONS))
	{
		m_fileVersion = enum_map_file_version::FILE_VERSION_NO_S3_FILE;
		m_error.AddError("Wrong map-file version %i", m_fileVersion);
		return;
	}

	loadMap(&FR, m_fileVersion);

}


//-------------------------------------//
bool clMapFileReader::loadMap(clFileReader * FR, enum_map_file_version fileVersion)
{
	int MapPartType = 0;
	unsigned int MapPartLen = 0;


	int FilePos = 8; //- start of map-content

	do
	{
		FR->setOffset(FilePos);

		MapPartType = FR->readIntBE(4);
		MapPartLen = FR->readIntBE(4);


		//- uncrypt
		MapPartLen = MapPartLen + 0xFFFFFFF8;

		unsigned int FileTypeSub = (MapPartType << 16) & 0x0000FFFF;
		MapPartType = MapPartType & 0x0000FFFF;

		int MapPartPos = FR->getOffset();

		m_error.AddDebug("@ %i - size:%i - Type:%i %i - [%s] ", FilePos, MapPartLen, MapPartType, FileTypeSub, getMapPartTypeName((enum_map_file_parts) MapPartType));

		//- next pos in File
		FilePos = FilePos + MapPartLen + 8;

		if ((MapPartType > 0) && (MapPartType < clMapFileReader::PART_TYPE____COUNT))
		{
			m_fileParts[MapPartType].cryptKey = MapPartType;
			m_fileParts[MapPartType].hasBeenDecrypted = false;
			m_fileParts[MapPartType].offset = MapPartPos;
			m_fileParts[MapPartType].size = MapPartLen;
		}

	}
	while ((MapPartType != 0) && (!FR->eof()));


	//- read some important information
	readMapInfo();
	readMapArea();



	return true;
}


//-------------------------------------//
bool clMapFileReader::readMapArea(unsigned int *outBuffer_AraeHeightObjects, int sizeOfBuffer_AraeHeightObject, unsigned int *outBuffer_AccessiblePlayerResources, int sizeOfBuffer_AccessiblePlayerResources)
{
	ty_file_part *FPart = &m_fileParts[enum_map_file_parts::PART_TYPE_Area];

	m_MapSizeWidth = m_MapSizeHeight = 0;

	if (FPart->size == 0)
	{
		m_error.AddError("No area-information available in mapfile!");
		return false;
	}

	clFileReader FR = clFileReader(&m_File, FPart->offset, FPart->size);
	doUncrypt(&FR, FPart);

	//- height and width are the same
	m_MapSizeHeight = m_MapSizeWidth = FR.readIntBE(4);

	int dataCount = m_MapSizeHeight * m_MapSizeWidth;
	const unsigned char * inBuffer = (const unsigned char *) FR.readBuffer(NULL, dataCount*6);


	if (inBuffer != NULL)
	{
		unsigned int * out1 = NULL;
		unsigned int * out2 = NULL;

		//- read the whol data
		if ((outBuffer_AraeHeightObjects != NULL) && (sizeOfBuffer_AraeHeightObject == dataCount)) out1 = outBuffer_AraeHeightObjects;
		if ((outBuffer_AccessiblePlayerResources != NULL) && (sizeOfBuffer_AccessiblePlayerResources == dataCount)) out2 = outBuffer_AccessiblePlayerResources;

		if ((out1 != NULL) || (out2 != NULL))
		{
			for (int i = dataCount; i > 0; i--)
			{
				const unsigned char v_height = *inBuffer++;
				const unsigned char v_Type = *inBuffer++;
				const unsigned char v_Object = *inBuffer++;

				if (out1)
				{
					int v = (v_height) | (v_Type << 8) | (v_Object << 16);
					*out1 = v;
					out1++;
				}

				const unsigned char v_plyerClaim = *inBuffer++;
				const unsigned char v_accessible = *inBuffer++;
				const unsigned char v_resources = *inBuffer++;

				if (out2)
				{
					*out2 = (v_accessible) | (v_plyerClaim << 8) || (v_resources << 16);
					out2++;
				}
			}
		}

	}
	else
	{
		m_error.AddError("Area-data-error: data size to small!");
		return false;
	}

	
	return true;
}



//-------------------------------------//
bool clMapFileReader::readMapInfo()
{
	ty_file_part *FPart = &m_fileParts[enum_map_file_parts::PART_TYPE_MapInfo];

	m_PlayerCount = 0;
	m_isSinglePlayerMap = true;


	if (FPart->size == 0)
	{
		m_error.AddError("No area-information available in mapfile!");
		return false;
	}

	clFileReader FR = clFileReader(&m_File, FPart->offset, FPart->size);
	doUncrypt(&FR, FPart);


	//- single or Multiplayer map?
	{
		int tmpVal = FR.readIntBE();
		if (tmpVal <= 1)
		{
			m_isSinglePlayerMap = (tmpVal == 1);
		}
		else
		{
			m_error.AddError("(readMapInfo) wrong value for 'isSinglePlayerMap' value: %i", tmpVal);
		}
	}


	m_PlayerCount = FR.readIntBE();
	if ((m_PlayerCount<1) || (m_PlayerCount>(sizeof(m_player)/sizeof(ty_player))))
	{
		m_error.AddError("(readMapInfo) wrong value for 'PlayerCount' ", m_PlayerCount);
		m_PlayerCount = 20;
	}


	m_startResources = (enum_STARTRESOURCES)FR.readIntBE();

	if ((m_startResources != enum_STARTRESOURCES::STARTRESOURCES_less) && (m_startResources != enum_STARTRESOURCES::STARTRESOURCES_medium) && (m_startResources != enum_STARTRESOURCES::STARTRESOURCES_much))
	{
		m_error.AddError("(readMapInfo) wrong value for 'StartResources' value: %i", m_startResources);
		m_startResources = enum_STARTRESOURCES::STARTRESOURCES_less;
	}


	//....
	return true;
}



//-------------------------------------//
SDL_Texture * clMapFileReader::getPreviewImage(SDL_Renderer* renderer, int shearFaktor)
{
	ty_file_part *FPart = &m_fileParts[enum_map_file_parts::PART_TYPE_Preview];

	if (FPart->size == 0)
	{
		m_error.AddError("No preview image available!");
		return NULL;
	}
	
	clFileReader FR = clFileReader(&m_File, FPart->offset, FPart->size);
	doUncrypt(&FR, FPart);

	//- read image
	int height = FR.readIntBE(2);
	int width = height;

	int outWidth = width;
	//- 0: don't shear; 1: 45° (every line); 2: 30° (every second line)
	if (shearFaktor > 0) outWidth += (height + (shearFaktor-1)) / shearFaktor;
	if (shearFaktor <= 0) shearFaktor = width + height;


	int unknown = FR.readIntBE(2);
	if (unknown != 0) m_error.AddError("(getPreviewImage) [unknown] header value %i != 0 ??", unknown);


	if (FPart->size < (width * height + 4))
	{
		m_error.AddError("(getPreviewImage) FilePartSize mismatch with the Preview Image width:%i height:%i.", outWidth, height);
		return NULL;
	}


	//- get buffer for new pixel data
	int *imgData = new int[outWidth * height];
	memset(imgData, 0, outWidth * height*sizeof(int));

	int *outData = imgData;


	int shearOffset = outWidth - width;
	int countShearCount = shearFaktor;



	for (int y = 0; y<height; y++)
	{
		outData = imgData + y*outWidth + shearOffset;

		countShearCount--;
		if (countShearCount <= 0)
		{
			countShearCount = shearFaktor;
			outData--; 
			shearOffset--;
		}
		

		for (int x = width; x > 0; x--)
		{
			int v = FR.readIntBE(2);

			//- scale colors to 255
			int b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
			int g = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 2) & 0xF8));
			int r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 7) & 0xF8));

			//-       Alpha                  Color
			*outData = 0x000000FF | (r << 24) | (g << 16) | (b << 8);
			outData++;
		}
	}



	//- create new Texture
	SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, outWidth, height);
	if (newTexture == 0)
	{
		m_error.AddError("(getPreviewImage) SDL_CreateTexture error: %s", SDL_GetError());
		return NULL;
	}

	//- turn off blending for this Texture
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);


	//- copy image data to Texture
	if (SDL_UpdateTexture(newTexture, NULL, imgData, outWidth * sizeof(int)) != 0)
	{
		delete imgData;
		m_error.AddError("(getPreviewImage) SDL_UpdateTexture error: %s", SDL_GetError());
		return NULL;
	}

	delete imgData;

	return newTexture;
	
}







//-------------------------------------//
bool clMapFileReader::doUncrypt(clFileReader * FR, ty_file_part *FPart)
{
	if (FPart->hasBeenDecrypted != 0) return true;

	int length = FR->length();
	if (length < 1) return true;


	FR->setOffset(0);

	//- key has to be byte
	unsigned char key = FPart->cryptKey & 0xFF;

	unsigned char * buffer = (unsigned char *) FR->DirectBufferAccess(length, 0);

	for (int i = length; i>0; i--)
	{
		//- read one byte
		unsigned char byt = *buffer ^ key;

		//- write Byte
		*buffer = byt;
		
		buffer++;

		//- calc Key
		key = ((key << 1) & 0xFF) ^ byt;
	}

	FPart->hasBeenDecrypted = true;
	
	FR->setOffset(0);

	return true;
}

//-------------------------------------//
const char * clMapFileReader::getMapPartTypeName(enum_map_file_parts partType)
{
	switch (partType & 0x0000FFFF)
	{
		case enum_map_file_parts::PART_TYPE_MapInfo: return "Map Info";
		case enum_map_file_parts::PART_TYPE_PlayerInfo: return "Player Info";
		case enum_map_file_parts::PART_TYPE_TeamInfo: return "Team Info";
		case enum_map_file_parts::PART_TYPE_Preview: return "Preview";
		case enum_map_file_parts::PART_TYPE_Area: return "Area";
		case enum_map_file_parts::PART_TYPE_Settlers: return "Settlers";
		case enum_map_file_parts::PART_TYPE_Buildings: return "Buildings";
		case enum_map_file_parts::PART_TYPE_Resources: return "Resources";
		case enum_map_file_parts::PART_TYPE_QuestText: return "QuestText";
		case enum_map_file_parts::PART_TYPE_QuestTip: return "QuestTip";
		case enum_map_file_parts::PART_TYPE_EOF: return "(EOF)";
	}

	return "?";
}

//-------------------------------------//
bool clMapFileReader::checkChecksum(clFileReader * FR)
{
	//- read checksum from file
	FR->setOffset(0);
	unsigned int fileCheckSum = FR->readIntBE();

	//- read 4Byte
	int count = (FR->length()/4) - 2;

	//- start to read
	FR->setOffset(8);
	
	unsigned int checksum = 0;

	for (int i = count; i > 0; i--)
	{
		unsigned int currentDWord = FR->readIntBE(4);
		checksum = ((checksum >> 31) | ((checksum << 1) ^ currentDWord));
	}

	//- move to next value in File
	FR->setOffset(4);

	if (checksum != fileCheckSum)
	{
		return false;
	}

	return true;
}


//-------------------------------------//
clMapFileReader::~clMapFileReader()
{
}
