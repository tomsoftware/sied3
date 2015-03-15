#include "absBuilding.h"


class clBuilding_r_smallTower : public absBuilding
{

private:

	clTextureAtlas::tyTextureAtlasPos m_TexturePos;
public:

	//-------------------------------------//
	void LoadResources(clObjectTextures *Textures, clGFXFile *gfxBuilding)
	{
		Textures->AddTexturePlaneSequenz(absBuilding::enumType::BUILDING_SMALL_TWOWER, gfxBuilding, 17, 20);
	}

	//-------------------------------------//
	void Draw(clScreen * Screen, tyBuilding *building, int screenPosX, int screenPosY)
	{
		Screen->addTexture(building->posX, building->posY, building->posX, building->posY, building->terrainHeight, absBuilding::enumType::BUILDING_SMALL_TWOWER, clScreen::enumTextureObjectTypes::TEXTURE_BUILDINGS_ROMAN);
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
		return enumType::BUILDING_SMALL_TWOWER;
	}

};

