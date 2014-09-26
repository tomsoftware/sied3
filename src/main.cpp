#include "main.h"


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		m_error.AddError("SDL_Init failed", SDL_GetError());
		return 1;
	}

	m_sdl_window = SDL_CreateWindow("Sied 3", 100, 100, 1300, 800, SDL_WINDOW_SHOWN);
	if (m_sdl_window == nullptr)
	{
		m_error.AddError("SDL_CreateWindow Error", SDL_GetError());
		SDL_Quit();
		return 1;
	}


	SDL_Renderer *renderer = SDL_CreateRenderer(m_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		SDL_DestroyWindow(m_sdl_window);
		m_error.AddError("SDL_CreateRenderer Error", SDL_GetError());
		SDL_Quit();
		return 1;
	}



	loadMap(renderer, "448_DEMO.map");
	loadResource(renderer);
	gameLoop(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(m_sdl_window);
	SDL_Quit();


	m_error.AddError("Terminate");

	
	return 0;
}


//-------------------------------------//
bool eventHandler()
{
	SDL_Event event; // event handler

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return false;
		}


		if (event.type == SDL_MOUSEMOTION)
		{
			if ((event.motion.state & SDL_BUTTON_LMASK) == SDL_BUTTON_LMASK)
			{
				m_mapPosX += event.motion.xrel;
				m_mapPosY += event.motion.yrel;

				if (m_mapPosX < 0) m_mapPosX = 0;
				if (m_mapPosY < 0) m_mapPosY = 0;

				//SDL_WarpMouseInWindow(m_sdl_window, event.motion.x - event.motion.xrel, event.motion.y - event.motion.yrel);
			}
			else
			{
				m_MouseDownStartX = -1;
				m_MouseDownStartY = -1;
			}
		}

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_KP_MINUS:
				AnimationID--;
				if (AnimationID < 0) AnimationID = 0;
				m_GameLoopCounter = 0;
				m_error.AddDebug("AnimationID: %i", AnimationID);
				break;
			case SDLK_KP_PLUS:
				m_GameLoopCounter = 0;
				AnimationID++;
				m_error.AddDebug("AnimationID: %i", AnimationID);
				break;
			case SDLK_PAGEUP:
				m_GameLoopCounter = 0;
				BuildingID++;
				m_error.AddDebug("BuildingID: %i", BuildingID);
				break;
			case SDLK_PAGEDOWN:
				m_GameLoopCounter  = 0;
				BuildingID--;
				if (BuildingID < 0) BuildingID = 0;
				m_error.AddDebug("BuildingID: %i", BuildingID);
				break;

			case SDLK_ESCAPE:
				return false;
				break;
			}
		}
	}

	return true;
}




//-------------------------------------//
void gameLoop(SDL_Renderer *renderer)
{
	bool gameRunning = true;
	
	

	int m_GameLoopCounter=0;
	int scale = 1;
	int x0 = 200;
	int y0 = 150;

	while (gameRunning)
	{
		gameRunning = eventHandler();

		//- clean Output
		SDL_RenderClear(renderer);

		//- draw Map
		drawMap(renderer, m_mapPosX, m_mapPosY);
		drawMapObjects(renderer, m_mapPosX, m_mapPosY);



		//////////////////////
		//- <Preview>
		SDL_Rect destPosMapPreview = { 1000, 50, 150 + 150/2, 150 };
		SDL_RenderCopy(renderer, mapPreview, NULL, &destPosMapPreview);
		//- </Preview>
		//////////////////////



		//////////////////////
		//- <Building>
		gfxBuilding.getTextureObject(&txBuild, renderer, BuildingID, BuildingID + 3, 0);
		
		SDL_Rect destPos1 = { (x0 + txBuild.xRel) * scale, (y0 + txBuild.yRel) * scale, txBuild.width * scale, txBuild.height * scale };
		SDL_RenderCopy(renderer, txBuild.image, NULL, &destPos1);

		clGFXFile::delete_GFX_Object(&txBuild);
		//- </Building>


		//////////////////////
		//- <Animation>
		int animCount = gfxAnimation.getAnimationInfoFrameCount(AnimationID);
		if (animCount > 0)
		{
			int animFrame = m_GameLoopCounter % animCount;


			clGFXFile::GFX_ObjectAnimationFrame gfxAniFrame;
			gfxAnimation.getAnimationInfo(&gfxAniFrame, AnimationID, animFrame);


			clGFXFile::GFX_ObjectTexture txNewSied;
			clGFXFile::GFX_ObjectSurface txNewTorso;

			if ((gfxAniFrame.sound_flag1 != -1) || (gfxAniFrame.sound_flag2 != -1))
			{
				m_error.AddDebug("sound: %i %i", gfxAniFrame.sound_flag1, gfxAniFrame.sound_flag2);
			}

			if (((gfxAniFrame.torso_file < 10) || (gfxAniFrame.object_file <10) || (gfxAniFrame.shadow_file < 10)) && ((gfxAniFrame.torso_file > 12) || (gfxAniFrame.object_file > 12) || (gfxAniFrame.shadow_file > 12)))
			{
				m_error.AddError("files: %i %i %i", gfxAniFrame.object_file, gfxAniFrame.torso_file, gfxAniFrame.shadow_file);
			}


			if (gfxAniFrame.object_file == 11)
			{
				gfxSied11.getTextureObject(&txNewSied, renderer, gfxAniFrame.object_id, gfxAniFrame.shadow_id, gfxAniFrame.object_frame);
				gfxSied11.getTextureTorso(&txNewTorso, renderer, gfxAniFrame.torso_id, gfxAniFrame.torso_frame);
			}
			else if(gfxAniFrame.object_file == 10)
			{
				gfxSied10.getTextureObject(&txNewSied, renderer, gfxAniFrame.object_id, gfxAniFrame.shadow_id, gfxAniFrame.object_frame);
				gfxSied10.getTextureTorso(&txNewTorso, renderer, gfxAniFrame.torso_id, gfxAniFrame.torso_frame);
			}
			else if (gfxAniFrame.object_file == 12)
			{
				gfxSied12.getTextureObject(&txNewSied, renderer, gfxAniFrame.object_id, gfxAniFrame.shadow_id, gfxAniFrame.object_frame);
				gfxSied12.getTextureTorso(&txNewTorso, renderer, gfxAniFrame.torso_id, gfxAniFrame.torso_frame);
			}

			drawObject(renderer, &txNewSied, &txNewTorso, x0 + gfxAniFrame.posX, y0 + gfxAniFrame.posY, scale);

			clGFXFile::delete_GFX_Object(&txNewSied);
			clGFXFile::delete_GFX_Object(&txNewTorso);

		}
		//////////////////////

		for (int c = 0; c < 10; c++)
		{
			int x = ((c % 10) * 35 + 35); //+ i*scale;
			int y = ((c / 10) * 35 + 35); //+ i*scale;

			int animFrame = m_GameLoopCounter % animWizzard.count;

			drawObject(renderer, &animWizzard.texture[animFrame], &animWizzard.torso[animFrame], x, y, scale);
		}

		

		SDL_RenderPresent(renderer);

		m_GameLoopCounter++;
		SDL_Delay(1000/25);
	}
}

//-------------------------------------//
void drawObject(SDL_Renderer *renderer, clGFXFile::GFX_ObjectTexture *texture, clGFXFile::GFX_ObjectSurface *torso, int x, int y, int scale)
{
	if (texture->image != NULL)
	{
		SDL_Rect destPos = { (texture->xRel + x)* scale, (texture->yRel + y) * scale, texture->width * scale, texture->height * scale };
		SDL_RenderCopy(renderer, texture->image, NULL, &destPos);
	}

	if (torso->image != NULL)
	{
		SDL_Rect destPos = { (torso->xRel + x) * scale, (torso->yRel + y) * scale, torso->width * scale, torso->height * scale };
		SDL_SetSurfacePalette(torso->image, palTorso);
		SDL_Texture * tmpTex = SDL_CreateTextureFromSurface(renderer, torso->image);
		SDL_SetTextureBlendMode(tmpTex, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(renderer, tmpTex, NULL, &destPos);
		SDL_DestroyTexture(tmpTex);
	}
}


//-------------------------------------//
void drawMap(SDL_Renderer *renderer, int posX, int posY)
{
	if (m_map_AraeHeightObject != NULL)
	{

		//unsigned int * pBuf0 = m_map_AraeHeightObject + ( posY)*m_mapWidth + posX;
		//int v0 = *pBuf0;
		//m_error.AddDebug("(%i,%i) = %i", posX, posY, (v0 >> 8) & 0xFF);


		for (int curY = 0; curY < 50; curY++)
		{
			int sumY = curY + posY;

			if ((sumY >= 0) && (sumY < m_mapHeight/2))
			{
				for (int x = 0; x < 70; x++)
				{
					int curX = x + curY ;
					int sumX = curX + posX-20;
					unsigned int * pRowData = m_map_AraeHeightObject + sumY*(m_mapWidth + m_mapHeight);

					if ((sumX > 0) && (sumX < (m_mapWidth)))
					{
						int v = *(pRowData + sumX);

						int area = (v >> 8) & 0xFF;
						int obj = (v >> 16) & 0xFF;
						
						int outX = curX * 16 - curY * 8 * 16 / 9;
						int outY = curY * 16;

						SDL_Rect posOut = { outX, outY, 16, 16 };
						SDL_Rect posIn;

						if (txLandscape[area].width == 128)
						{
							posIn = { (sumX % 8) * 16, (sumY % 8) * 9, 16, 9 };
						}
						else
						{
							posIn = { 0, 0, 16, 16 };
						}

						SDL_RenderCopy(renderer, txLandscape[area].image, &posIn, &posOut);

					}
				}
			}
		}

	}
}


//-------------------------------------//
void drawMapObjects(SDL_Renderer *renderer, int posX, int posY)
{
	if (m_map_AraeHeightObject != NULL)
	{

		//unsigned int * pBuf0 = m_map_AraeHeightObject + (posY) *m_mapWidth + posX;
		//int v0 = *pBuf0;
		//m_error.AddDebug("(%i,%i) = %i", posX, posY, (v0 >> 8) & 0xFF);


		for (int curY = 0; curY < 50; curY++)
		{
			int sumY = curY + posY;

			if ((sumY >= 0) && (sumY < m_mapHeight / 2))
			{
				for (int x = 0; x < 70; x++)
				{
					int curX = x + curY;
					int sumX = curX + posX - 20;
					unsigned int * pRowData = m_map_AraeHeightObject + sumY*(m_mapWidth + m_mapHeight);

					if ((sumX > 0) && (sumX < (m_mapWidth)))
					{
						int v = (*(pRowData + sumX)) & 0xFF0000;

						if (v != 0)
						{
							int obj = (v >> 16);

							if (txObjects[obj].image != NULL)
							{
								int outX = curX * 16 - curY * 8 * 16 / 9;
								int outY = curY * 16;

								SDL_Rect posOut = { outX, outY, txObjects[obj].width, txObjects[obj].height };





								SDL_RenderCopy(renderer, txObjects[obj].image, NULL, &posOut);
							}
							else
							{
								m_error.AddDebug("obj=%i", obj);
							}

						}
					}
				}
			}
		}

	}
}
//-------------------------------------//
void loadMap(SDL_Renderer *renderer, const char * fileName)
{
	clMapFileReader map = clMapFileReader(clMapFileReader::enum_map_folders::FOLDER_USER, fileName);

	m_mapWidth = map.getMapWidth();
	m_mapHeight = map.getMapHeight();

	m_error.AddDebug("Map size w:%i h:%i", m_mapWidth, m_mapHeight);

	//- load Preview Image 
	mapPreview = map.getPreviewImage(renderer, 2);


	//- destroy old map buffer
	if (m_map_AraeHeightObject != NULL)
	{
		delete m_map_AraeHeightObject;
		m_map_AraeHeightObject = NULL;
	}
	if (m_map_AccessiblePlayerResources != NULL)
	{
		delete m_map_AccessiblePlayerResources;
		m_map_AccessiblePlayerResources = NULL;
	}

	int bufferSize = m_mapWidth*m_mapHeight;

	//- create ne map buffer
	//- MapInfos: AreaType, Height, Object
	m_map_AraeHeightObject = new unsigned int[bufferSize];

	//- MapInfo: Accessibilety, Player, Resources
	m_map_AccessiblePlayerResources = new unsigned int[bufferSize];
	
	//- read map informations
	map.readMapArea(m_map_AraeHeightObject, bufferSize, m_map_AccessiblePlayerResources, bufferSize);
}





//-------------------------------------//
void loadResource(SDL_Renderer *renderer)
{


	//- Landscape
	clGFXFile gfxLand = clGFXFile("Siedler3_00.f8007e01f[ori].dat");

	int count = gfxLand.getTextureLandscapeCount();


	gfxLand.getTextureLandscape(&txLandscape[7], renderer, 10); //- Meer [OK]
	gfxLand.getTextureLandscape(&txLandscape[16], renderer, 0); //- gras [OK]
	gfxLand.getTextureLandscape(&txLandscape[32], renderer, 21); //- Fels[OK]
	gfxLand.getTextureLandscape(&txLandscape[48], renderer, 31); //- Strand[OK]
	gfxLand.getTextureLandscape(&txLandscape[64], renderer, 18); //- Wüste [OK]
	gfxLand.getTextureLandscape(&txLandscape[80], renderer, 7); //- Sumpf [OK]
	gfxLand.getTextureLandscape(&txLandscape[128], renderer, 24); //- Eis [OK]
	gfxLand.getTextureLandscape(&txLandscape[144], renderer, 4); //- Schlamm / Totes land [OK]
	gfxLand.getTextureLandscape(&txLandscape[20], renderer, 20);
	gfxLand.getTextureLandscape(&txLandscape[65], renderer, 19);
	gfxLand.getTextureLandscape(&txLandscape[17], renderer, 22); //24,23
	gfxLand.getTextureLandscape(&txLandscape[33], renderer, 23);

	gfxLand.getTextureLandscape(&txLandscape[35], renderer, 165);
	gfxLand.getTextureLandscape(&txLandscape[129], renderer, 166);

	//- Objects
	clGFXFile gfxObjects = clGFXFile("Siedler3_01.f8007e01f.dat");

	clGameObjects::load_game_objects(renderer, gfxObjects, txObjects);

	//clGameObjects::load_game_buildings_roman(renderer, gfxObjects, txBuildings);


	//- Animation skripts
	gfxAnimation.openGFXFile("Siedler3_15.f8007e01f.dat");

	//- Buildings
	gfxBuilding.openGFXFile("Siedler3_13.f8007e01f.dat");

	//- Sägewerk
	//gfxBuilding.getTextureObject(&txBuild, renderer, 1, 4, 0);

	//- Forsthaus
	//gfxBuilding.getTextureObject(&txBuild, renderer, 0, 3, 0);


	gfxSied10.openGFXFile("Siedler3_10.f8007e01f.dat");
	gfxSied11.openGFXFile("Siedler3_11.f8007e01f.dat");
	gfxSied12.openGFXFile("Siedler3_12.f8007e01f.dat");

	//- kein plan
	gfxSied11.getTextureObject(&txSied, renderer, 188, 1, 0);
	gfxSied11.getTextureTorso(&txTorso, renderer, 175, 0);


	clGFXFile gfxPalette = clGFXFile("Siedler3_01.f8007e01f.dat");
	palTorso = gfxPalette.getPalette(0, 7);


	int FrameCount = 72;


	animWizzard.count = FrameCount;
	animWizzard.texture = new clGFXFile::GFX_ObjectTexture[animWizzard.count];
	animWizzard.torso = new clGFXFile::GFX_ObjectSurface[animWizzard.count];

	for (int i = 0; i < FrameCount; i++)
	{
		gfxSied11.getTextureObject(&animWizzard.texture[i], renderer, 188, 188, i);
		gfxSied11.getTextureTorso(&animWizzard.torso[i], renderer, 175, i);
	}


	for (int i = 0; i < FrameCount; i++)
	{
		gfxSied11.getTextureObject(&animWizzard.texture[i], renderer, 172, 172, i);
		gfxSied11.getTextureTorso(&animWizzard.torso[i], renderer, 159, i);
	}
}
