#include "main.h"


int main(int argc, char* argv[])
{
	if (!initWindow()) return -1;


	//loadMap("texture.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	loadMap("448_DEMO.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	//loadMap( "flach.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	loadResource();

	gameLoop();


	unloadWindow();


	m_error.AddError("Terminate");

	
	return 0;
}


//-------------------------------------//
void unloadWindow()
{
	glfwDestroyWindow(m_window);

	m_window = NULL;

	glfwTerminate();
}


//-------------------------------------//
bool initWindow()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		m_error.AddError("Error initWindow:glfwInit()");
		return false;
	}


	m_window = glfwCreateWindow(1024, 640, "Sied3", NULL, NULL);
	if (!m_window)
	{
		m_error.AddError("glfwCreateWindow() Failed.");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	//- Registrate Callbacks
	glfwSetKeyCallback(m_window, key_callback);

	glfwSetMouseButtonCallback(m_window, mouse_click_callback);

	glfwSetCursorPosCallback(m_window, mouse_move_callback);
	return true;

}

//-------------------------------------//
void setUpCam()
{
	float ratio;
	int width = 0;
	int height = 0;

	glfwGetFramebufferSize(m_window, &width, &height);

	ratio = width / (float) height;

	/*
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	*/

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, width, 0.f, height, 1.f, -1.f);
}




//-------------------------------------//
bool eventHandler()
{
	//glfwPollEvents();

	//if (glfwWindowShouldClose(m_window)) return false;

	//if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) return false;



	/*

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
	*/

	return true;
}




//-------------------------------------//
void gameLoop()
{
	bool gameRunning = true;
	
	
	int m_GameLoopCounter=0;
	int scale = 1;
	int x0 = 200;
	int y0 = 150;




	while (gameRunning)
	{
		gameRunning = eventHandler();

		//- set up View Point
		setUpCam();


		//- clean Output

		//- draw Map
		drawMap(m_mapPosX, m_mapPosY);
		drawMapObjects(m_mapPosX, m_mapPosY);


		/*
		//////////////////////
		//- <Preview>
		SDL_Rect destPosMapPreview = { 1000, 50, 315, 209 };
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
				//m_error.AddDebug("sound: %i %i", gfxAniFrame.sound_flag1, gfxAniFrame.sound_flag2);
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

		*/

		//glfwSwapBuffers(m_window);
		glfwSwapBuffers(m_window);
		glfwPollEvents();



		m_GameLoopCounter++;
		//sleep(1000);
	}
}
/*
//-------------------------------------//
void drawObject(clGFXFile::GFX_ObjectTexture *texture, clGFXFile::GFX_ObjectSurface *torso, int x, int y, int scale)
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
*/


//-------------------------------------//
void drawMap(int posX, int posY)
{
	float textOffsetX1 = 0.f;
	float textOffsetY1 = 0.f;
	float textOffsetX2 = 0.f;
	float textOffsetY2 = 0.f;
	int outX = 0;
	int outY = 0;
	int curArea = -1; //- avoid switching textures if not necessary

	//- setup OpenGL
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glLoadIdentity();



	//- the heigh of one line
	static const int YSTEP = (16 * 9 / 8) / 2;

	for (int y = 0; y < 68; y++)
	{
		outX = (y % 2) * 8;
		int sumY = posY + y;

		if ((sumY<m_mapHeight) && (sumY>=0))
		{
			unsigned int * pRowData = m_map_AraeHeightObject + (m_mapHeight - sumY)*m_mapWidth;

			for (int x = 0; x < 60; x++)
			{
				int sumX = posX + x - y/2;

				if ((sumX < m_mapWidth) && (sumX >= 0))
				{
					int v = *(pRowData + sumX);

					int area = (v >> 8) & 0xFF;

					//if (area == m_marker) area = 1;
					if ((area == m_marker) || (txLandscape[area].texture != NULL))
					{

						if (curArea != area)
						{
							glBindTexture(GL_TEXTURE_2D, txLandscape[area].texture);
							curArea = area;
						}

						glBegin(GL_TRIANGLES);

						//--      1 --- 4  ^
						//--     / \ B /   |
						//--    / A \ /    |YSTEP
						//--   2 --- 3     v

						switch (txLandscape[area].textType)
						{
							case clOpenGLTexturesHelper::TEXTURE_TYPE_PLANE:
								//- X: after 4 Lines in y-direction, X increases by 128/2
								textOffsetX1 = ((sumX + ((sumY / 4) % 2) * 4) % 8) * 16.f / 128.f + (sumY % 8)* 8.f / 128.f;
								textOffsetY1 = 16.f / 128.f * (sumY % 8);

								textOffsetX2 = textOffsetX1 + 8.f / 128.f;
								textOffsetY2 = textOffsetY1;
								break;

							case clOpenGLTexturesHelper::TEXTURE_TYPE_2x2_HEXAGON:
								textOffsetX1 = 0.5f;
								textOffsetY1 = 0.f;
								textOffsetX2 = 0.5f;
								textOffsetY2 = 0.5f;
								break;
						}

						////// A /////
						//-- ->1
						glTexCoord2f(textOffsetX1 + 8.f / 128.f, textOffsetY1 + 16.f / 128.f);
						glVertex3f(outX + 8, outY + YSTEP, 0.f);

						//-- ->2
						glTexCoord2f(textOffsetX1, textOffsetY1);
						glVertex3f(outX, outY, 0.f);

						//-- ->3
						glTexCoord2f(textOffsetX1 + 16.f / 128.f, textOffsetY1);
						glVertex3f(outX + 16, outY, 0.f);

						////// B /////
						//-- ->1
						glTexCoord2f(textOffsetX2, textOffsetY2 + 16.f / 128.f);
						glVertex3f(outX + 8, outY + YSTEP, 0.f);

						//-- ->3
						glTexCoord2f(textOffsetX2 + 8.f / 128.f, textOffsetY2);
						glVertex3f(outX + 16, outY, 0.f);

						//-- ->4
						glTexCoord2f(textOffsetX2 + 16.f / 128.f, textOffsetY2 + 16.f / 128.f);
						glVertex3f(outX + 8 + 16, outY + YSTEP, 0.f);

						glEnd();

					}
				}
				outX += 16;
			}
		}
		outY += YSTEP;
	}


	glDisable(GL_TEXTURE_2D);

}




//-------------------------------------//
void drawMapObjects(int posX, int posY)
{

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	static const int YSTEP = (16 * 9 / 8) / 2;
	int outX = 0;
	int outY = YSTEP * 68;

	

	for (int y = 68; y > 0; y--)
	{
		outX = (y % 2) * 8;
		int sumY = posY + y;

		if ((sumY<m_mapHeight) && (sumY >= 0))
		{
			unsigned int * pRowData = m_map_AraeHeightObject + (m_mapHeight - sumY)*m_mapWidth;

			for (int x = 0; x < 60; x++)
			{
				int sumX = posX + x - y / 2;

				if ((sumX < m_mapWidth) && (sumX >= 0))
				{
					int v = *(pRowData + sumX);

					int obj = (v >> 16) & 0xFF;

					if ((obj != 0) && (txObjects[obj].texture != NULL))
					{

						//--   1 --- 4
						//--   |     |
						//--   |     |
						//--   2 --- 3

						glBindTexture(GL_TEXTURE_2D, txObjects[obj].texture);
						glBegin(GL_QUADS);


						int curX = txObjects[obj].xRel + outX;
						int curY = txObjects[obj].yRel + outY + txObjects[obj].height;
						int curW = txObjects[obj].width;
						int curH = txObjects[obj].height;

						////// A /////
						//-- ->1
						glTexCoord2f(0,0);
						glVertex2f(curX, curY + curH);

						//-- ->2
						glTexCoord2f(0, 1);
						glVertex2f(curX, curY);

						//-- ->3
						glTexCoord2f(1, 1);
						glVertex2f(curX + curW, curY);

						//-- ->4
						glTexCoord2f(1, 0);
						glVertex2f(curX + curW, curY + curH);


						glEnd();


					}
				}
				outX += 16;
			}
		}
		outY -= YSTEP;
	}


	glEnd();

	glDisable(GL_TEXTURE_2D);
}




//-------------------------------------//
void loadMap(const char * fileName, clMapFileReader::enum_map_folders  mapType)
{
	clMapFileReader map;

	map.readMap(mapType, fileName);


	m_mapWidth = map.getMapWidth();
	m_mapHeight = map.getMapHeight();

	m_error.AddDebug("Map size w:%i h:%i", m_mapWidth, m_mapHeight);

	//- load Preview Image 
	//mapPreview = map.getPreviewImage(2);


	//- destroy old map buffer
	if (m_map_AraeHeightObject != NULL)
	{
		delete m_map_AraeHeightObject;
		m_map_AraeHeightObject = NULL;
	}
	if (m_map_AccessiblePlayerResources != NULL)
	{
		delete[] m_map_AccessiblePlayerResources;
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
void loadResource()
{

	//- Landscape
	clGFXFile gfxLand = clGFXFile("Siedler3_00.f8007e01f.dat");

	int count = gfxLand.getTextureLandscapeCount();

	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[7], &gfxLand, 10); //- see [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[16], &gfxLand, 0); //- grass [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[32], &gfxLand, 21); //- rock [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[48], &gfxLand, 31); //- beach [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[64], &gfxLand, 18); //- desert [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[80], &gfxLand, 7); //- swamp [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[128], &gfxLand, 24); //- Polar [OK]
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX(&txLandscape[144], &gfxLand, 4); //- mud [OK]

	//- between the different map-textures a blending is created. 
	//-  for this blending the outer-part of the feld (e.g. grass=0 -> 128) is changed PLUS two map-rows (20,65) and then the outer-part of the dessert
	//-  to make the blend more cloudy the are always two images for the same purpose (e.g. {128 | 129})
	//- for 16-->64: [ {0}, {128 | 129} , {130 | 131}, {132 | 133} , {134 | 135}, {136 | 137} ,  {138 | 139}, {18}   <--- texture-file-index
	//--             |      16          |           20             |             65           |         64           <--- map-type-index
	//--                   grass                                                                      desert         <---  "name"
	//-
	//-- to improve handling of the blending and the variation of textures, the textures for one map-type-index are copyed side by side in one texture:
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX2x2(&txLandscape[20], &gfxLand, 130, 131, 132, 133, 16, 65); //- 128 129 130 131
	clOpenGLTexturesHelper::loadLandscapeTextureFromGFX2x2(&txLandscape[65], &gfxLand, 134, 135, 136, 137, 20, 64); //- 128 129 130 131
	

	//gfxLand.getTextureLandscape(&txLandscape[20], renderer, 20);
	//gfxLand.getTextureLandscape(&txLandscape[65], renderer, 19);
	//gfxLand.getTextureLandscape(&txLandscape[17], renderer, 22); //24,23
	//gfxLand.getTextureLandscape(&txLandscape[33], renderer, 23);

	//gfxLand.getTextureLandscape(&txLandscape[35], renderer, 165);
	//gfxLand.getTextureLandscape(&txLandscape[129], renderer, 166);


	//- Objects
	clGFXFile gfxObjects = clGFXFile("Siedler3_01.f8007e01f.dat");

	clOpenGLTexturesHelper::load_game_objects(gfxObjects, txObjects);


	/*
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
	
	*/
}



//=============================================================================
// CALLBACKS
//=============================================================================

//-------------------------------------//
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{

		switch (key)
		{
			case GLFW_KEY_ESCAPE: // ESCAPE
				exit(0);
				break;
			case GLFW_KEY_KP_ADD:
				m_marker++;
				m_error.AddDebug("Marker: %i", m_marker);
				break;
			case GLFW_KEY_KP_SUBTRACT:
				m_marker--;
				m_error.AddDebug("Marker: %i", m_marker);
				break;
			case 'd': // switch rendering modes (fill -> wire -> point)
			case 'D':
				drawMode = ++drawMode % 3;
				if (drawMode == 0)        // fill mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					//glEnable(GL_DEPTH_TEST);
					glEnable(GL_CULL_FACE);
				}
				else if (drawMode == 1)  // wireframe mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					//glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
				}
				else                    // point mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					//glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
				}
				break;
		}
	}
}

//-------------------------------------//
void mouse_click_callback(GLFWwindow *window, int button, int action, int mods)
{
	double mouseX = 0;
	double mouseY = 0;

	glfwGetCursorPos(window, &mouseX, &mouseY);
	

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			m_MouseDownStartX = mouseX - m_mapPosX;
			m_MouseDownStartY = m_mapPosY - m_mapHeight + mouseY;

			mouseLeftDown = true;
		}
		else if (action == GLFW_RELEASE)
			mouseLeftDown = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			mouseRightDown = true;
		}
		else if (action == GLFW_RELEASE)
			mouseRightDown = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			mouseMiddleDown = true;
		}
		else if (action == GLFW_RELEASE)
			mouseMiddleDown = false;
	}
}

//-------------------------------------//
void mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (mouseLeftDown)
	{
		m_mapPosX = xpos - m_MouseDownStartX;
		m_mapPosY = m_mapHeight - (ypos - m_MouseDownStartY);


		if (m_mapPosX < 0) m_mapPosX = 0;
		if (m_mapPosY < 0) m_mapPosY = 0;

		if (m_mapPosX> m_mapWidth) m_mapPosX = m_mapWidth;
		if (m_mapPosY> m_mapHeight) m_mapPosY = m_mapHeight;
	}
}

//-------------------------------------//
static void error_callback(int error, const char* description)
{
	m_error.AddError("GLFW: [%i] %s", error, description);
	fputs(description, stderr);
}


