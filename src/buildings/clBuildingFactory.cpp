#ifndef BuildingFactory_CPP
#define BuildingFactory_CPP

#include "clBuilding_r_smallTower.cpp"
#include "clBuilding_r_charburner.cpp"
#include "clBuilding_r_bladesmith.cpp"
#include "../clMapFileReader.h"

class clBuildingFactory
{
private:
	clBuilding_r_smallTower m_small_tower = clBuilding_r_smallTower();
	clBuilding_r_charburner m_charburner = clBuilding_r_charburner();
	clBuilding_r_bladesmith m_bladesmith = clBuilding_r_bladesmith();

public:

	//-------------------------------------//
	void LoadResources(clObjectTextures *Textures, clGFXFile *gfxBuilding)
	{
		m_small_tower.LoadResources(Textures, gfxBuilding);
		m_charburner.LoadResources(Textures, gfxBuilding);
		m_bladesmith.LoadResources(Textures, gfxBuilding);
	}

	//-------------------------------------//
	clBuilding_r_smallTower * get_SmallTower()
	{
		return &m_small_tower;
	}

	//-------------------------------------//
	clBuilding_r_charburner * get_Charburner()
	{
		return &m_charburner;
	}


	//-------------------------------------//
	absBuilding * getBuildingClassByMapType(clMapFileReader::enumBType buildingType)
	{
		switch (buildingType)
		{
			case clMapFileReader::enumBType::BType_TurmKleiner: return & m_small_tower;
			case clMapFileReader::enumBType::BType_Koehler: return &m_charburner;
			case clMapFileReader::enumBType::BType_Waffenschmiede: return &m_bladesmith;
		}

		return NULL;
	}

};


#endif