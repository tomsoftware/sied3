#include "clGameObjects.h"


clGameObjects::clGameObjects()
{
}


clGameObjects::~clGameObjects()
{
}


void clGameObjects::load_game_objects(SDL_Renderer *renderer, clGFXFile &gfxObjects, clGFXFile::GFX_ObjectTexture txObjects[255])
{
	//- 0: Stacks

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_1],		renderer, 1, 1, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_2],		renderer, 2, 2, 0);

	//- 3: ELM fall

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_1],		renderer, 4, 4, 0);
	//gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_2],	renderer, 5, 5, 0); //- same as [GAME_OBJECT_TREE_ELM_2]... not used in Map

	//- 6: OAK fall

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_1],	renderer, 7, 7, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_2],	renderer, 8, 8, 0);

	//- 9: BIRCH fall

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_1],	renderer, 10, 10, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_2],	renderer, 11, 11, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_3],	renderer, 13, 13, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_4],	renderer, 14, 14, 0);



	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_1],	renderer, 16, 16, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_2],	renderer, 17, 17, 0);



	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_SMALL],			renderer, 22, 22, 0);



	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_1],		renderer, 27, 26, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_2],		renderer, 27, 26, 1);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_3],		renderer, 27, 26, 2);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_1],			renderer, 27, 26, 3);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_2],			renderer, 27, 26, 4);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_3],			renderer, 27, 26, 5);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_1],		renderer, 27, 26, 6);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_2],		renderer, 27, 26, 7);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_1],			renderer, 27, 26, 8);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_2],			renderer, 27, 26, 9);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_1],			renderer, 27, 26, 10);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_2],			renderer, 27, 26, 11);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_3],			renderer, 27, 26, 12);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_4],			renderer, 27, 26, 13);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BONES],				renderer, 27, 26, 14);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_1],			renderer, 27, 26, 15);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_2],			renderer, 27, 26, 16);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_3],			renderer, 27, 26, 17);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_1],		renderer, 27, 26, 18);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_2],		renderer, 27, 26, 19);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_3],		renderer, 27, 26, 20);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_4],		renderer, 27, 26, 21);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_1],				renderer, 27, 26, 22);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_2],				renderer, 27, 26, 23);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_3],				renderer, 27, 26, 24);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_4],				renderer, 27, 26, 25);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STRUB_5],				renderer, 27, 26, 26);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_1],			renderer, 27, 26, 27);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_2],			renderer, 27, 26, 28);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_3],			renderer, 27, 26, 29);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_4],			renderer, 27, 26, 30);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_5],			renderer, 27, 26, 31);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_6],			renderer, 27, 26, 32);


	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_WRECK_1], renderer, 28, 27, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_WRECK_2], renderer, 28, 27, 1);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_WRECK_3], renderer, 28, 27, 2);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_WRECK_4], renderer, 28, 27, 3);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_WRECK_5], renderer, 28, 27, 4);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_GRAVE],	renderer, 28, 27, 5);


	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_1], renderer, 29, 28, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_2], renderer, 29, 28, 1);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_3], renderer, 29, 28, 2);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_4], renderer, 29, 28, 3);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_5], renderer, 29, 28, 4);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_6], renderer, 29, 28, 5);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_7], renderer, 29, 28, 6);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_8], renderer, 29, 28, 7);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STONE_1], renderer, 29, 28, 8);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STONE_2], renderer, 29, 28, 9);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STONE_3], renderer, 29, 28, 10);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_STONE_4], renderer, 29, 28, 11);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_1], renderer, 29, 28, 12);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_2], renderer, 29, 28, 13);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_3], renderer, 29, 28, 14);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_4], renderer, 29, 28, 15);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_5], renderer, 29, 28, 16);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_6], renderer, 29, 28, 17);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_7], renderer, 29, 28, 18);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_8], renderer, 29, 28, 19);

	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REEF_SMALL], renderer,	29, 28, 20);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REEF_MEDIUM], renderer,	29, 28, 21);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REEF_LARGE], renderer,	29, 28, 22);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_REEF_XLARGE], renderer,	29, 28, 23);


	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_1], renderer, 30, 29, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_2], renderer, 30, 29, 1);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_3], renderer, 30, 29, 2);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_4], renderer, 30, 29, 3);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_5], renderer, 30, 29, 4);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_6], renderer, 30, 29, 5);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_7], renderer, 30, 29, 6);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_8], renderer, 30, 29, 7);




	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_01], renderer, 31, 30, 0);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_02], renderer, 31, 30, 1);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_03], renderer, 31, 30, 2);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_04], renderer, 31, 30, 3);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_05], renderer, 31, 30, 4);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_06], renderer, 31, 30, 5);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_07], renderer, 31, 30, 6);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_08], renderer, 31, 30, 7);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_09], renderer, 31, 30, 8);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_10], renderer, 31, 30, 9);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_11], renderer, 31, 30, 10);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_12], renderer, 31, 30, 11);
	gfxObjects.getTextureObject(&txObjects[enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_13], renderer, 31, 30, 12);
}



/*
void load_game_buildings_roman(SDL_Renderer *renderer, clGFXFile &gfxObjects, clGFXFile::GFX_ObjectTexture txObjects[255])
{

	return
}
*/