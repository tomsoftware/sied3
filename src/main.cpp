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
	bool quit = false;
	
	SDL_Event event; // event handler

	int i=0;

	while (!quit)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				quit = true;
			}
		}
		i++;




		SDL_RenderCopy(renderer, txBack.image, NULL, NULL);

		SDL_Rect destPosMapPreview = { 0, 0, 150 + 150/2, 150 };
		SDL_RenderCopy(renderer, mapPreview, NULL, &destPosMapPreview);
		

		int scale = 2;


		for (int c = 0; c < 10; c++)
		{

			int x = ((c % 10) * 35 + 35)*scale; //+ i*scale;
			int y = ((c / 10) * 35 + 35)*scale; //+ i*scale;
			

			i = i % animWizzard.count;
			clGFXFile::GFX_ObjectSurface torso = animWizzard.torso[i];
			clGFXFile::GFX_ObjectTexture texture = animWizzard.texture[i];


			SDL_Rect destPos1 = { texture.xRel * scale + x, texture.yRel * scale + y, texture.width * scale, texture.height * scale };
			SDL_RenderCopy(renderer, texture.image, NULL, &destPos1);
		

			SDL_Rect destPos2 = { (torso.xRel) * scale + x, torso.yRel * scale + y, torso.width * scale, torso.height * scale };
			SDL_SetSurfacePalette(torso.image, palTorso);
			SDL_Texture * tmpTex = SDL_CreateTextureFromSurface(renderer, torso.image);
			SDL_SetTextureBlendMode(tmpTex, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(renderer, tmpTex, NULL, &destPos2);
			SDL_DestroyTexture(tmpTex);
		}


		SDL_Rect destPos1 = { 100, 100, txBuild.width * scale, txBuild.height * scale };
		SDL_RenderCopy(renderer, txBuild.image, NULL, &destPos1);


		SDL_RenderPresent(renderer);


		SDL_Delay(1000/25);
	}
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
	clGFXFile gfxLand = clGFXFile("Siedler3_00.7c003e01f.dat");

	gfxLand.getTextureLandscape(&txBack, renderer, 0);



	clGFXFile gfxBuilding = clGFXFile("siedler3_13.7c003e01f.dat");

	//- Sägewerk
	gfxBuilding.getTextureObject(&txBuild, renderer, 1, 4, 0);

	//- Forsthaus
	gfxBuilding.getTextureObject(&txBuild, renderer, 0, 3, 0);



	clGFXFile gfxSied = clGFXFile("siedler3_11.7c003e01f.dat");
	//- kein plan
	gfxSied.getTextureObject(&txSied, renderer, 188, 1, 0);
	gfxSied.getTextureTorso(&txTorso, renderer, 175, 0);


	clGFXFile gfxPalette = clGFXFile("siedler3_01.7c003e01f.dat");
	palTorso = gfxPalette.getPalette(0, 7);



	animWizzard.count = 72;
	animWizzard.texture = new clGFXFile::GFX_ObjectTexture[animWizzard.count];
	animWizzard.torso = new clGFXFile::GFX_ObjectSurface[animWizzard.count];

	for (int i = 0; i < 72; i++)
	{
		gfxSied.getTextureObject(&animWizzard.texture[i], renderer, 188, 188, i);
		gfxSied.getTextureTorso(&animWizzard.torso[i], renderer, 175, i);
	}


	for (int i = 0; i < 72; i++)
	{
		gfxSied.getTextureObject(&animWizzard.texture[i], renderer, 172, 172, i);
		gfxSied.getTextureTorso(&animWizzard.torso[i], renderer, 159, i);
	}
}