#ifndef CLMAPFILEREADER_H
#define CLMAPFILEREADER_H

#include "clError.h"
#include "clFile.h"
#include "clConfig.h"


class clMapFileReader
{

public:
	enum enum_map_folders
	{
		FOLDER_NONE,
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

	/// - Settler Types
	enum enumSType
	{
		SType_Traeger = 0,
		SType_Planierer = 1,
		SType_Bauarbeiter = 2,
		SType_Holzfaeller = 3,
		SType_Steinmetz = 4,
		SType_Swordsman1 = 5,
		SType_Saegewerker = 6,
		SType_Foerster = 7,
		SType_Archer1 = 8,
		SType_Schmelzer = 9,
		SType_Minenarbeiter = 10,
		SType_Schmied = 11,
		SType_Mueller = 12,
		SType_Baecker = 13,
		SType_Metzger = 14,
		SType_SpearFighter1 = 15,
		SType_Bauer = 16,
		SType_Fischer = 17,
		SType_Wasserwerker = 18,
		SType_Werftarbeiter = 19,
		//'-20 unknown
		SType_Heiler = 21,
		SType_Geologe = 22,
		SType_Dieb = 23,
		SType_Koehler = 24,
		SType_Schnapsbrenner = 25,
		SType_Brauer = 26,
		SType_Pulvermacher = 27,
		SType_Schweinefarmer = 28,
		SType_Winzer = 29,
		SType_Archer2 = 30,
		SType_SpearFighter2 = 31,
		SType_Swordsman2 = 32,
		SType_Archer3 = 33,
		SType_SpearFighter3 = 34,
		SType_Swordsman3 = 35,
		SType_Prister = 36,
		//- 37 Unknown
		SType_Reisbauer = 38,
		SType_Esel = 39,
		SType_Pionier = 40,
		SType_Katapult = 41,
		//- 42 Unknown
		SType_Handelsschiff = 43,
		SType_Faehre = 44,
		SType_Imker = 45,
		SType_Metwinzer = 46,
		SType_Alchemist = 47
	};


	///- Building types
	enum enumBType
	{
		BType_Lager = 1,
		BType_Holzfaeller = 2,
		BType_Steinbruch = 3,
		BType_Saegewerk = 4,
		BType_Foersterhuette = 5,
		BType_Beobachtungsturm = 6,
		BType_Kohlemine = 7,
		BType_Goldmine = 8,
		BType_Eisenmine = 9,
		BType_Goldschmelze = 10,
		BType_Eisenschmelze = 11,
		BType_Werkzeugschmiede = 12,
		BType_Waffenschmiede = 13,
		BType_Weingut = 14,
		BType_TurmKleiner = 15,
		BType_TurmGrosser = 16,
		BType_Muehle = 17,
		BType_TurmBurg = 18,
		BType_Kasserne = 19,
		BType_Baeckerei = 20,
		BType_Metzger = 21,
		BType_Destille = 22,
		BType_Schweinefarm = 23,
		BType_Getreidefram = 24,
		BType_Fischerhuette = 25,
		BType_WohnhausKlein = 26,
		BType_WohnhausMittel = 27,
		BType_WohnhausGross = 28,
		BType_Schwefelmine = 29,
		BType_Wasserwerk = 30,
		BType_Katapultwerk = 31,
		BType_Werft = 32,
		BType_Anlegestelle = 33,
		BType_Marktplatz = 34,
		BType_Heiler = 35,
		BType_Reisfarm = 36,
		BType_Edelsteinmine = 37,
		BType_Brauerei = 38,
		BType_Koehler = 39,
		BType_Pulvermacherei = 40,
		BType_Pyramide = 41,
		BType_Sphinx = 42,
		BType_GrosserTempel = 43,
		BType_kleinerTempel = 44,
		BType_grosse_Pagode = 45,
		BType_kleine_Pagode = 46,
		BType_Ballistenwerkstatt = 47,
		BType_Kanonenwerkstatt = 48,
		BType_Eselfarm = 49,
		BType_grosser_Gong = 50,
		BType_Impkerei = 51,
		BType_Metwinzerei = 52,
		BType_Labortorium = 53,
		BType_kleiner_Tempel = 54,
		BType_grosser_Tempel = 55,
		BType_SchwefelmineAmazonen = 56
	};


	struct ty_Settler
	{
		int x_pos; //- word
		int y_pos; //- word
		enumSType SType;  //- byte
		int party; //- Byte
	};


	struct ty_Building
	{
		unsigned char party; //- byte
		enumBType BType; //- byte
		int x_pos; //- word
		int y_pos; //- word

		int unknown;

		int SoldierCount;
		//ty_Settler Soldier[];
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
	clMapFileReader(enum_map_folders map_folder=FOLDER_NONE, const char * fileName="");
	void readMap(enum_map_folders map_folder, const char * fileName);

	//SDL_Texture * getPreviewImage(SDL_Renderer* renderer, int shearFaktor=0);
	~clMapFileReader();
	int getMapWidth() { return m_MapSizeWidth; };
	int getMapHeight() { return m_MapSizeHeight; };

	ty_Building * readMapBuildings(int * BuildingsCount);
	bool readMapArea(unsigned int *outBuffer_AraeHeightObjects = NULL, int sizeOfBuffer_AraeHeightObject = 0, unsigned int *outBuffer_PlayerAccessibleResources = NULL, int sizeOfBuffer_PlayerAccessibleResources = 0);
};

#endif