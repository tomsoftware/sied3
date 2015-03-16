#include "absBuilding.h"


class clBuilding_r_charburner : public absBuilding
{

private:

	int m_TexturePos = 0;
public:

	//-------------------------------------//
	void LoadResources(clObjectTextures *Textures, clGFXFile *gfxBuilding)
	{
		Textures->createTextureInformation(absBuilding::enumType::BUILDING_CHARBURNER, 3);
		m_TexturePos = Textures->setTextureInformation(absBuilding::enumType::BUILDING_CHARBURNER, 0, gfxBuilding, 30, 27, 0);
	}

	//-------------------------------------//
	void Draw(clScreen * Screen, tyBuilding *building)
	{
		Screen->addTexture(building->posX, building->posY, building->posX, building->posY, building->terrainHeight, m_TexturePos, clScreen::enumTextureObjectTypes::TEXTURE_BUILDINGS_ROMAN);
	}

	//-------------------------------------//
	void Tick(tyBuilding *building)
	{

	}

	//-------------------------------------//
	void init(tyBuilding *building, int MapPos_x, int MapPos_y, unsigned char terrainHeight)
	{
		building->posX = MapPos_x;
		building->posY = MapPos_y;
		building->terrainHeight = terrainHeight;
	}

	//-------------------------------------//
	void Create(tyBuilding *building, int x, int y)
	{

	}

	//-------------------------------------//
	void Destroy(tyBuilding *building)
	{

	}

	//-------------------------------------//
	enumType getType()
	{
		return enumType::BUILDING_CHARBURNER;
	}

};

