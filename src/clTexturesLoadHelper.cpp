#include "clTexturesLoadHelper.h"





//-------------------------------------//
void clTexturesLoadHelper::load_map_patterns(clLandscapeTextures *objectTextures, clGFXFile *gfxLand)
{
	objectTextures->AddTexturePlane128x128(gfxLand, 10, 7); //- sea
	objectTextures->AddTexturePlane128x128(gfxLand, 0, 16); //- grass
	objectTextures->AddTexturePlane128x128(gfxLand, 21, 32); //- rock
	objectTextures->AddTexturePlane128x128(gfxLand, 31, 48); //- beach
	objectTextures->AddTexturePlane128x128(gfxLand, 18, 64); //- desert
	objectTextures->AddTexturePlane128x128(gfxLand, 7, 80); //- swamp
	objectTextures->AddTexturePlane128x128(gfxLand, 24, 128); //- polar/ice
	objectTextures->AddTexturePlane128x128(gfxLand, 4, 144); //- mud


	//- between the different map-textures a blending is created.
	//- for this a index-table is created containing all known edge-type-combinations

	//- grass <--> desert
	objectTextures->AddTextureHexagon(gfxLand, 128, 129, 130, 131, 16, 20); //- grass 16 <-x-> 20 <---> 65 <---> desert 64
	objectTextures->AddTextureHexagon(gfxLand, 132, 133, 134, 135, 20, 65); //- grass 16 <---> 20 <-x-> 65 <---> desert 64
	objectTextures->AddTextureHexagon(gfxLand, 136, 137, 138, 139, 65, 64); //- grass 16 <---> 20 <---> 65 <---> desert 64


	//- see [level 0] <--> see [level 7]
	objectTextures->AddTextureHexagon(gfxLand, 84, 85, 86, 87, 0, 1);
	objectTextures->AddTextureHexagon(gfxLand, 88, 89, 90, 91, 1, 2);
	objectTextures->AddTextureHexagon(gfxLand, 92, 93, 94, 95, 2, 3);
	objectTextures->AddTextureHexagon(gfxLand, 96, 97, 98, 99, 3, 4);
	objectTextures->AddTextureHexagon(gfxLand, 100, 101, 102, 103, 4, 5);
	objectTextures->AddTextureHexagon(gfxLand, 104, 105, 106, 107, 5, 6);
	objectTextures->AddTextureHexagon(gfxLand, 108, 109, 110, 111, 6, 7);


	//- sea constant (no Gradient) level
	objectTextures->AddTexturePlane32x32(gfxLand, 17, 0);
	objectTextures->AddTexturePlane32x32(gfxLand, 16, 1);
	objectTextures->AddTexturePlane32x32(gfxLand, 15, 2);
	objectTextures->AddTexturePlane32x32(gfxLand, 14, 3);
	objectTextures->AddTexturePlane32x32(gfxLand, 13, 4);
	objectTextures->AddTexturePlane32x32(gfxLand, 12, 5);
	objectTextures->AddTexturePlane32x32(gfxLand, 11, 6);

	//- sea to beach
	objectTextures->AddTextureHexagon(gfxLand, 37, 38, 39, 40, 48, 0);

	//- beach to grass
	objectTextures->AddTextureHexagon(gfxLand, 112, 113, 114, 115, 48, 16);

	//- [grass] 16 -> 17 -> 33 -> 32 [rock]
	objectTextures->AddTextureHexagon(gfxLand, 116, 117, 118, 119, 16, 17);
	objectTextures->AddTextureHexagon(gfxLand, 120, 121, 122, 123, 17, 33);
	objectTextures->AddTextureHexagon(gfxLand, 124, 125, 126, 127, 33, 32);

	//- [rock]
	objectTextures->AddTexturePlane32x32(gfxLand, 22, 33);
	objectTextures->AddTexturePlane32x32(gfxLand, 23, 17);


	//- [grass] 16 -> 23 -> 145 -> 144 [mud]
	objectTextures->AddTextureHexagon(gfxLand, 140, 141, 142, 143, 16, 23);
	objectTextures->AddTextureHexagon(gfxLand, 144, 145, 146, 147, 23, 145);
	objectTextures->AddTextureHexagon(gfxLand, 148, 149, 150, 151, 145, 144);


	//- [rock] 32 -> 35 -> 129 -> 128 [ice]
	objectTextures->AddTextureHexagon(gfxLand, 156, 157, 158, 159, 32, 35);
	objectTextures->AddTextureHexagon(gfxLand, 160, 161, 162, 163, 35, 129);
	objectTextures->AddTextureHexagon(gfxLand, 164, 165, 166, 167, 129, 128);
	//---> ?? 27,28,29,30

	//- [ice]
	objectTextures->AddTexturePlane32x32(gfxLand, 25, 35);
	objectTextures->AddTexturePlane32x32(gfxLand, 26, 129);


	//-  [grass] 16 -> 21 -> 81 -> 80 [swamp]
	objectTextures->AddTextureHexagon(gfxLand, 201, 202, 203, 204, 16, 21);
	objectTextures->AddTextureHexagon(gfxLand, 205, 206, 207, 208, 21, 81);
	objectTextures->AddTextureHexagon(gfxLand, 209, 210, 211, 212, 81, 80);

	//- [swamp]
	objectTextures->AddTexturePlane32x32(gfxLand, 8, 21);
	objectTextures->AddTexturePlane32x32(gfxLand, 9, 81);

	//- desert
	objectTextures->AddTexturePlane32x32(gfxLand, 19, 65);
	objectTextures->AddTexturePlane32x32(gfxLand, 20, 20);


	//- grass -> river
	objectTextures->AddTextureHexagon(gfxLand, 52, 53, 54, 55, 16, 96);
	objectTextures->AddTextureHexagon(gfxLand, 56, 57, 58, 59, 16, 97);
	objectTextures->AddTextureHexagon(gfxLand, 60, 61, 62, 63, 16, 98);
	objectTextures->AddTextureHexagon(gfxLand, 64, 65, 66, 67, 16, 99);

	//- beach [48] -> river
	objectTextures->AddTextureHexagon(gfxLand, 68, 69, 70, 71, 48, 96);
	objectTextures->AddTextureHexagon(gfxLand, 72, 73, 74, 75, 48, 97);
	objectTextures->AddTextureHexagon(gfxLand, 76, 77, 78, 79, 48, 98);
	objectTextures->AddTextureHexagon(gfxLand, 80, 81, 82, 83, 48, 99);

	/*
	//- riffer -> sea
	objectTextures->AddTextureMapping(0, 0, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(0, 0, 96, 0, 0, 0);
	//objectTextures->AddTextureHexagon(gfxLand, 84, 85, 86, 87, 0, 99);
	//objectTextures->AddTextureHexagon(gfxLand, 84, 85, 86, 87, 0, 96);


	objectTextures->AddTextureMapping(16, 96, 97, 16, 97, 97);
	objectTextures->AddTextureMapping(16, 97, 96, 16, 97, 97);

	objectTextures->AddTextureMapping(16, 96, 99, 16, 98, 98);
	objectTextures->AddTextureMapping(16, 99, 96, 16, 98, 98);

	objectTextures->AddTextureMapping(16, 97, 98, 16, 98, 98);
	objectTextures->AddTextureMapping(16, 98, 99, 16, 99, 99);

	objectTextures->AddTextureMapping(0, 96, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(0, 97, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(0, 98, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(0, 99, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(96, 0, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(97, 0, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(98, 0, 99, 0, 0, 0);
	objectTextures->AddTextureMapping(99, 0, 99, 0, 0, 0);



	objectTextures->AddTextureMapping(48, 96, 0, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 0, 96, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 97, 0, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 0, 97, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 98, 0, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 0, 98, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 99, 0, 48, 0, 0);
	objectTextures->AddTextureMapping(48, 0, 99, 48, 0, 0);
	*/




}

//-------------------------------------//
void clTexturesLoadHelper::load_game_objects(clObjectTextures *objectTextures, clGFXFile *gfxObjects)
{
	//- 0: Stacks

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_1, gfxObjects, 1, 1);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_1, gfxObjects, 2, 2);


	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_1, gfxObjects, 1, 1);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ELM_2, gfxObjects, 2, 2);

	//- 3: ELM fall

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_1, gfxObjects, 4, 4);
	
	//- same as [GAME_OBJECT_TREE_ELM_2]... not used in Map
	//objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_OAK_2, gfxObjects,	renderer, 5, 5); 
	

	//- 6: OAK fall

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_1, gfxObjects, 7, 7);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_BIRCH_2, gfxObjects, 8, 8);

	//- 9: BIRCH fall

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_1, gfxObjects, 10, 10);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_2, gfxObjects, 11, 11);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_3, gfxObjects, 13, 13);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_4, gfxObjects, 14, 14);

	

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_1, gfxObjects, 16, 16);
	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_ARECACEAE_2, gfxObjects, 17, 17);

	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_UNKNOWN_5, gfxObjects, 19, 19); //-- or maybe a baby tree??!!


	objectTextures->AddTexturePlaneSequenz(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_SMALL, gfxObjects, 22, 22);

	//-- wather waves

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_1, gfxObjects, 27, 26, 0);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_2, gfxObjects, 27, 26, 1);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_PLANT_SMALL_3, gfxObjects, 27, 26, 2);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_1, gfxObjects, 27, 26, 3);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_2, gfxObjects, 27, 26, 4);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_MUSHROOM_3, gfxObjects, 27, 26, 5);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_1, gfxObjects, 27, 26, 6);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_STUMP_2, gfxObjects, 27, 26, 7);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_1, gfxObjects, 27, 26, 8);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_TREE_DEAD_2, gfxObjects, 27, 26, 9);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_1, gfxObjects, 27, 26, 10);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_2, gfxObjects, 27, 26, 11);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_3, gfxObjects, 27, 26, 12);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_CACTUS_4, gfxObjects, 27, 26, 13);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BONES, gfxObjects, 27, 26, 14);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_1, gfxObjects, 27, 26, 15);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_2, gfxObjects, 27, 26, 16);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_FLOWER_3, gfxObjects, 27, 26, 17);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_1, gfxObjects, 27, 26, 18);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_2, gfxObjects, 27, 26, 19);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_3, gfxObjects, 27, 26, 20);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_SMALL_4, gfxObjects, 27, 26, 21);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_1, gfxObjects, 27, 26, 22);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_2, gfxObjects, 27, 26, 23);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_3, gfxObjects, 27, 26, 24);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_4, gfxObjects, 27, 26, 25);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STRUB_5, gfxObjects, 27, 26, 26);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_1, gfxObjects, 27, 26, 27);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_2, gfxObjects, 27, 26, 28);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_3, gfxObjects, 27, 26, 29);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_4, gfxObjects, 27, 26, 30);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_5, gfxObjects, 27, 26, 31);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REED_BEDS_6, gfxObjects, 27, 26, 32);


	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_1, gfxObjects, 28, 27, 0);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_2, gfxObjects, 28, 27, 1);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_3, gfxObjects, 28, 27, 2);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_4, gfxObjects, 28, 27, 3);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_WRECK_5, gfxObjects, 28, 27, 4);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_GRAVE, gfxObjects, 28, 27, 5);


	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_1, gfxObjects, 29, 28, 0);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_2, gfxObjects, 29, 28, 1);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_3, gfxObjects, 29, 28, 2);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_4, gfxObjects, 29, 28, 3);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_5, gfxObjects, 29, 28, 4);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_6, gfxObjects, 29, 28, 5);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_7, gfxObjects, 29, 28, 6);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BIG_STONE_8, gfxObjects, 29, 28, 7);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_1, gfxObjects, 29, 28, 8);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_2, gfxObjects, 29, 28, 9);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_3, gfxObjects, 29, 28, 10);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_STONE_4, gfxObjects, 29, 28, 11);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_1, gfxObjects, 29, 28, 12);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_2, gfxObjects, 29, 28, 13);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_3, gfxObjects, 29, 28, 14);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_4, gfxObjects, 29, 28, 15);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_5, gfxObjects, 29, 28, 16);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_6, gfxObjects, 29, 28, 17);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_7, gfxObjects, 29, 28, 18);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_BOUNDERY_STONE_8, gfxObjects, 29, 28, 19);

	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_SMALL, gfxObjects, 29, 28, 20);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_MEDIUM, gfxObjects, 29, 28, 21);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_LARGE, gfxObjects, 29, 28, 22);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_REEF_XLARGE, gfxObjects, 29, 28, 23);


	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_1, gfxObjects, 30, 29, 0);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_2, gfxObjects, 30, 29, 1);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_3, gfxObjects, 30, 29, 2);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_4, gfxObjects, 30, 29, 3);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_5, gfxObjects, 30, 29, 4);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_6, gfxObjects, 30, 29, 5);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_7, gfxObjects, 30, 29, 6);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_SMALL_STONE_8, gfxObjects, 30, 29, 7);




	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_01, gfxObjects, 31, 30, 0);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_02, gfxObjects, 31, 30, 1);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_03, gfxObjects, 31, 30, 2);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_04, gfxObjects, 31, 30, 3);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_05, gfxObjects, 31, 30, 4);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_06, gfxObjects, 31, 30, 5);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_07, gfxObjects, 31, 30, 6);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_08, gfxObjects, 31, 30, 7);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_09, gfxObjects, 31, 30, 8);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_10, gfxObjects, 31, 30, 9);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_11, gfxObjects, 31, 30, 10);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_12, gfxObjects, 31, 30, 11);
	objectTextures->AddTexturePlane(clGameObjects::enum_GAME_OBJECTS::GAME_OBJECT_RES_STONE_13, gfxObjects, 31, 30, 12);

}


