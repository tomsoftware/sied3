#include "main.h"


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		m_error.AddError("SDL_Init failed", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Sied 3", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		m_error.AddError("SDL_CreateWindow Error", SDL_GetError());
		SDL_Quit();
		return 1;
	}


	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		SDL_DestroyWindow(win);
		m_error.AddError("SDL_CreateRenderer Error", SDL_GetError());
		SDL_Quit();
		return 1;
	}



	loadMap(renderer, "flach.map");
	loadResource(renderer);
	gameLoop(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();


	m_error.AddError("Terminate");

	
	return 0;
}


void gameLoop(SDL_Renderer *renderer)
{
	bool gameRunning = true;
	
	SDL_Event event; // event handler

	int i=0;

	while (gameRunning)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				gameRunning = false;
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_KP_MINUS:
					AnimationID--;
					i = 0;
					m_error.AddDebug("AnimationID: %i", AnimationID);
					break;
				case SDLK_KP_PLUS:
					i = 0;
					AnimationID++;
					m_error.AddDebug("AnimationID: %i", AnimationID);
					break;
				case SDLK_ESCAPE:
					gameRunning = false;
					break;
				}
			}
		}



		drawMap(renderer,10,10);


		SDL_RenderCopy(renderer, txBack.image, NULL, NULL);

		SDL_Rect destPosMapPreview = { 0, 0, 150 + 150/2, 150 };
		SDL_RenderCopy(renderer, mapPreview, NULL, &destPosMapPreview);
		

		int scale = 2;
		

		//////////////////////
		int animCount = gfxAnimation.getAnimationInfoFrameCount(AnimationID);
		if (animCount > 0)
		{
			int animFrame = i % animCount;


			clGFXFile::GFX_ObjectAnimationFrame gfxAniFrame;
			gfxAnimation.getAnimationInfo(&gfxAniFrame, AnimationID, animFrame);


			clGFXFile::GFX_ObjectTexture txNewSied;
			clGFXFile::GFX_ObjectSurface txNewTorso;

			if ((gfxAniFrame.flag3 != -1) || (gfxAniFrame.flag4 != -1))
			{
				m_error.AddDebug("flags: %i %i", gfxAniFrame.flag3, gfxAniFrame.flag4);
			}

			if ((gfxAniFrame.torso_file != 11) || (gfxAniFrame.object_file != 11) || (gfxAniFrame.shadow_file != 11))
			{
				m_error.AddError("files: %i %i %i", gfxAniFrame.object_file, gfxAniFrame.torso_file, gfxAniFrame.shadow_file);
			}

			gfxSied.getTextureObject(&txNewSied, renderer, gfxAniFrame.object_id, gfxAniFrame.shadow_id, gfxAniFrame.object_frame);
			gfxSied.getTextureTorso(&txNewTorso, renderer, gfxAniFrame.torso_id, gfxAniFrame.torso_frame);

			drawObject(renderer, &txNewSied, &txNewTorso, 200 + gfxAniFrame.posX, 150 + gfxAniFrame.posY, scale);

		}
		//////////////////////

		for (int c = 0; c < 10; c++)
		{

			int x = ((c % 10) * 35 + 35); //+ i*scale;
			int y = ((c / 10) * 35 + 35); //+ i*scale;
			

			int animFrame = i % animWizzard.count;

			drawObject(renderer, &animWizzard.texture[animFrame], &animWizzard.torso[animFrame], x, y, scale);
		}


		SDL_Rect destPos1 = { 100, 100, txBuild.width * scale, txBuild.height * scale };
		SDL_RenderCopy(renderer, txBuild.image, NULL, &destPos1);


		SDL_RenderPresent(renderer);


		i++;
		SDL_Delay(1000/25);
	}
}

//-------------------------------------//
void drawObject(SDL_Renderer *renderer, clGFXFile::GFX_ObjectTexture *texture, clGFXFile::GFX_ObjectSurface *torso, int x, int y, int scale)
{
	SDL_Rect destPos = { (texture->xRel + x)* scale, (texture->yRel+y) * scale , texture->width * scale, texture->height * scale };
	SDL_RenderCopy(renderer, texture->image, NULL, &destPos);


	destPos = { (torso->xRel+x) * scale, (torso->yRel + y) * scale, torso->width * scale, torso->height * scale };
	SDL_SetSurfacePalette(torso->image, palTorso);
	SDL_Texture * tmpTex = SDL_CreateTextureFromSurface(renderer, torso->image);
	SDL_SetTextureBlendMode(tmpTex, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, tmpTex, NULL, &destPos);
	SDL_DestroyTexture(tmpTex);
}


//-------------------------------------//
void drawMap(SDL_Renderer *renderer, int outX, int outY)
{
	int outW = 20 + outX;
	int outH = 20 + outY;



	for (int y = outY; y < outH; y++)
	{
		int * pBuf = m_map_AraeHeightObject + outY*m_mapWidth;

		for (int x = outX; x < outW; x++)
		{
			//int v = *(pBuf + outX);

		}
	}
	//m_mapWidth;


	
}


//-------------------------------------//
void loadMap(SDL_Renderer *renderer, const char * fileName)
{
	clMapFileReader map = clMapFileReader(clMapFileReader::enum_map_folders::FOLDER_USER, fileName);

	int m_mapWidth = map.getMapWidth();
	int m_mapHeight = map.getMapHeight();

	m_error.AddDebug("Map size w:%i h:%i", m_mapWidth, m_mapHeight);

	//- load Preview Image 
	mapPreview = map.getPreviewImage(renderer, 2);



	int *m_map_AraeHeightObject = NULL;
	int *m_map_AccessiblePlayerResources = NULL;


	int bufferSize = m_mapWidth*m_mapHeight;

	//- MapInfos: AreaType, Height, Object
	m_map_AraeHeightObject = new int[bufferSize];

	//- MapInfo: Accessibilety, Player, Resources
	m_map_AccessiblePlayerResources = new int[bufferSize];
	
	//- read map informations
	map.readMapArea(m_map_AraeHeightObject, bufferSize, m_map_AraeHeightObject, bufferSize);
}



//-------------------------------------//
void loadResource(SDL_Renderer *renderer)
{


	//- Landscape
	clGFXFile gfxLand = clGFXFile("Siedler3_00.7c003e01f.dat");
	gfxLand.getTextureLandscape(&txBack, renderer, 0);

	//- Animation skripts
	gfxAnimation.openGFXFile("Siedler3_15.f8007e01f.dat");


	clGFXFile gfxBuilding = clGFXFile("siedler3_13.7c003e01f.dat");

	//- Sägewerk
	gfxBuilding.getTextureObject(&txBuild, renderer, 1, 4, 0);

	//- Forsthaus
	gfxBuilding.getTextureObject(&txBuild, renderer, 0, 3, 0);


	gfxSied.openGFXFile("siedler3_11.7c003e01f.dat");



	gfxSied.openGFXFile("siedler3_11.7c003e01f.dat");
	//- kein plan
	gfxSied.getTextureObject(&txSied, renderer, 188, 1, 0);
	gfxSied.getTextureTorso(&txTorso, renderer, 175, 0);


	clGFXFile gfxPalette = clGFXFile("siedler3_01.7c003e01f.dat");
	palTorso = gfxPalette.getPalette(0, 7);


	int FrameCount = 72;


	animWizzard.count = FrameCount;
	animWizzard.texture = new clGFXFile::GFX_ObjectTexture[animWizzard.count];
	animWizzard.torso = new clGFXFile::GFX_ObjectSurface[animWizzard.count];

	for (int i = 0; i < FrameCount; i++)
	{
		gfxSied.getTextureObject(&animWizzard.texture[i], renderer, 188, 188, i);
		gfxSied.getTextureTorso(&animWizzard.torso[i], renderer, 175, i);
	}


	for (int i = 0; i < FrameCount; i++)
	{
		gfxSied.getTextureObject(&animWizzard.texture[i], renderer, 172, 172, i);
		gfxSied.getTextureTorso(&animWizzard.torso[i], renderer, 159, i);
	}
}