#ifndef BuildingFactory_CPP
#define BuildingFactory_CPP

#include "clBuilding_r_smallTower.cpp"
#include "clMapFileReader.h"

class clBuildingFactory
{
private:
	clBuilding_r_smallTower m_small_tower = clBuilding_r_smallTower();

public:

	//-------------------------------------//
	void LoadResources(clObjectTextures *Textures, clGFXFile *gfxBuilding)
	{
		m_small_tower.LoadResources(Textures, gfxBuilding);
	}

	//-------------------------------------//
	clBuilding_r_smallTower * get_SmallTower()
	{
		return &m_small_tower;
	}

	//-------------------------------------//
	absBuilding * getBuildingClassByMapType(clMapFileReader::enumBType buildingType)
	{
		switch (buildingType)
		{
		case clMapFileReader::enumBType::BType_TurmKleiner: return & m_small_tower;
		}

		return NULL;
	}

};


#endif