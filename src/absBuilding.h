#ifndef absBuilding_H
#define absBuilding_H

#include "openGL.h"

#include "clGFXFile.h"
#include "clObjectTextures.h"
#include "clScreen.h"

//-http://strategywiki.org/wiki/The_Settlers_III/Buildings


class absBuilding {
public:

	enum enumType
	{
		BUILDING_WOODCUTTER,
		BUILDING_SMALL_TWOWER
	};

	enum enumState
	{
		BUILDING_NULL,
		BUILDING_DISABLED,
		BUILDING_FLATTING,
		BUILDING_BUILD,
		BUILDING_WAIT_FOR_WORKER,
		BUILDING_WAIT_FOR_STACK,
		BUILDING_NO_MORE_RESOURCES,
		BUILDING_WORK,
		BUILDING_PAUSE,
		BUILDING_DESTROY
	};

	struct tyBuilding
	{
		/// <summary>Interpreter for this Item</summary>
		absBuilding * ProcessorClass = 0;
		/// <summary>Map Position X</summary>
		int posX;
		/// <summary>Map Position Y</summary>
		int posY;
		/// <summary>Stack Index of the first Stack: there can be more then one but they have to be subsequent</summary>
		int StackIndex = 0;
		/// <summary>settler who works here</summary>
		int SettlerIndex = 0;
		/// <summary>state of the building</summary>
		enumState State = BUILDING_NULL;
		/// <summary>sub state e.g. timer for e.g. animation</summary>
		int subState = 0;
		/// <summary>terrain height of the building</summary>
		unsigned char terrainHeight = 0;
	};

	/// <summary>Load the game resources for this kind of building e.g. Texture, Animation, Sound</summary>
	virtual void LoadResources(clObjectTextures *Textures, clGFXFile *gfxBuilding) = 0;

	/// <summary>initialise this [tyBuilding] Building record</summary>
	virtual void init(tyBuilding *building, int MapPos_x, int MapPos_y, unsigned char terrainHeight) = 0;

	/// <summary>draw this building to screen</summary>
	virtual void Draw(clScreen * Screen, tyBuilding *building, int screenPosX, int screenPosY) = 0;

	/// <summary>work... do the next step e.g. next Frame</summary>
	virtual void Tick(tyBuilding *building) = 0;

	/// <summary>create a new building</summary>
	virtual void Create(tyBuilding *building, int x, int y) = 0;

	/// <summary>start destroy this building</summary>
	virtual void Destroy(tyBuilding *building) = 0;
	
	/// <summary>return the type of this Building</summary>
	virtual enumType getType() = 0;
	
};

#endif