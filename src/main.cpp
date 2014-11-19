#include "main.h"


int main(int argc, char* argv[])
{
	if (!initGLContext()) return -1;


	loadResource();

	//loadMap("texture.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	//loadMap("448_DEMO.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	//loadMap("hoch.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	//loadMap( "flach.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	//loadMap("wueste_gras_wueste.map", clMapFileReader::enum_map_folders::FOLDER_USER);
	
	loadMap("M704-2-Longrun.map", clMapFileReader::enum_map_folders::FOLDER_MULTI);
	//loadMap("leer1.map", clMapFileReader::enum_map_folders::FOLDER_USER);

	loadShader();

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
bool initGLContext()
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

	//- init glew functions
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		m_error.AddError("glewInit() Failed: %s", glewGetErrorString(err));
	}


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
void gameLoop()
{
	bool gameRunning = true;
	
	
	m_GameLoopCounter=0;
	int scale = 1;
	int x0 = 200;
	int y0 = 150;




	while (gameRunning)
	{
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

		glfwSwapBuffers(m_window);
		glfwPollEvents();


		//- ToDo: Loop count by time
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
	//- texture position in textureAtlas
	float textX1 = 0.f;
	float textY1 = 0.f;
	float textX2 = 0.f;
	float textY2 = 0.f;

	//- size of map texture triangles
	const float textSizeWidth = 16.0f / m_LandscapeText->getWidth();
	const float textSizeHeight = 16.0f / m_LandscapeText->getHeight();
	const float textSizeWidthHalf = textSizeWidth * 0.5f;

	//- screen coordinates for output
	int outX = 0;
	int outY = 0;

	//- the heigh of one line
	static const int YSTEP = (16 * 9 / 8) / 2;


	//- setup OpenGL
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glLoadIdentity();

	//- use the texture atlas Texture
	glBindTexture(GL_TEXTURE_2D, m_LandscapeText->getGLTextureId());

	//- use the map-shader
	m_map_shader.activate();



	glBegin(GL_TRIANGLES);



	for (int y = 0; y < 68; y++)
	{
		outX = (y % 2) * 8;
		int sumY = posY + y;

		if ((sumY<m_mapHeight) && (sumY>=0))
		{
			ty_mapLandscape * pRowData = m_map_landscape + (m_mapHeight - sumY - 1)*m_mapWidth;

			for (int x = 0; x < 60; x++)
			{
				int sumX = posX + x - y/2;

				if ((sumX < m_mapWidth) && (sumX >= 0))
				{
					ty_mapLandscape * l = (pRowData + sumX);

					int textMapPosA = l->textureMapPosA;
					int textMapPosB = l->textureMapPosB;

					//- for debugging we can switch one map-types to white 
					if (l->AraeType == m_marker)
					{
						textMapPosA = m_MarkerTextureTypeID;
						textMapPosB = m_MarkerTextureTypeID;
					}


					ty_textureMapPos * textA = &m_texturePos[textMapPosA];
					ty_textureMapPos * textB = &m_texturePos[textMapPosB];

					int textureType = textA->textureType;

					if ((textureType == clLandscapeTextures::enumTextureType::TEXTURE_TYPE_SINGEL))
					{
						if ((sumX + sumY / 2) % 2)
						{
							//- use variant 1
							textX1 = textA->textureA_var1_X;
							textY1 = textA->textureA_var1_Y;

							textX2 = textB->textureB_var1_X;
							textY2 = textB->textureB_var1_Y;
						}
						else
						{
							//- use variant 2
							textX1 = textA->textureA_var2_X;
							textY1 = textA->textureA_var2_Y;

							textX2 = textB->textureB_var2_X;
							textY2 = textB->textureB_var2_Y;
						}


					}
					else if ((textureType == clLandscapeTextures::enumTextureType::TEXTURE_TYPE_REPEAT128x128))
					{
						//textX1 = l.textureA_var1_X + (((sumX + ((sumY / 4) % 2) * 4) % 8) * 16.f + (sumY % 8)* 8.f) / texWidth;
						//- toDo: switch every 8th row

						ty_textureMapPos *tmp = &m_texturePos[l->textureMapPosA];

						textX1 = tmp->textureA_var1_X + textSizeWidth * ((sumX + sumY / 2) % 8);
						textY1 = tmp->textureA_var1_Y + textSizeHeight * (sumY % 8);

						textX2 = textX1 + textSizeWidthHalf;
						textY2 = textY1;
					}

					unsigned char H1 = l->AraeHeight1;
					unsigned char H2 = l->AraeHeight2;
					unsigned char H3 = l->AraeHeight3;
					unsigned char H4 = l->AraeHeight4;
					
					//- disable the Height information
					if (!m_useHeight) H1 = H2 = H3 = H4 = 0;


					if (textureType != clLandscapeTextures::enumTextureType::TEXTURE_TYPE_NOT_FOUND)
					{

						//--      1 --- 4  ^
						//--     / \ B /   |
						//--    / A \ /    |YSTEP
						//--   2 --- 3     v

						////// A /////
						//-- ->1

						float color = GRADIENT_COLOR[l->gradientA1];
						glColor3f(color, color, color);

						glTexCoord2f(textX1 + textSizeWidthHalf, textY1 + textSizeHeight);
						glVertex3i(outX + 8, outY + YSTEP + H1, 0);

						//-- ->2
						color = GRADIENT_COLOR[l->gradientA2];
						glColor3f(color, color, color);

						glTexCoord2f(textX1, textY1);
						glVertex3i(outX, outY + H2, 0);

	
						//-- ->3
						color = GRADIENT_COLOR[l->gradientA3];
						glColor3f(color, color, color);

						glTexCoord2f(textX1 + textSizeWidth, textY1);
						glVertex3i(outX + 16, outY + H3, 0);




						////// B /////
						//-- ->1
						color = GRADIENT_COLOR[l->gradientA1];
						glColor3f(color, color, color);

						glTexCoord2f(textX2, textY2 + textSizeHeight);
						glVertex3i(outX + 8, outY + YSTEP + H1, 0);

						//-- ->3
						color = GRADIENT_COLOR[l->gradientA3];
						glColor3f(color, color, color);

						glTexCoord2f(textX2 + textSizeWidthHalf, textY2);
						glVertex3i(outX + 16, outY + H3, 0);

						//-- ->4
						color = GRADIENT_COLOR[l->gradientA4];
						glColor3f(color, color, color);

						glTexCoord2f(textX2 + textSizeWidth, textY2 + textSizeHeight);
						glVertex3i(outX + 8 + 16, outY + YSTEP + H4, 0);
					}
				}
				outX += 16;
			}
		}
		outY += YSTEP;
	}

	//- disable/reset color
	glColor3f(1,1,1);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	//- disable map-shader
	m_map_shader.disable();
}



//-------------------------------------//
void drawMapObjects(int posX, int posY)
{
	//- screen position for Object
	static const int YSTEP = (16 * 9 / 8) / 2;
	int outX = 0;
	int outY = YSTEP * 68;


	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, m_ObjectText->getGLTextureId());


	
	int gameLoopPos = m_GameLoopCounter;


	glBegin(GL_QUADS);

	for (int y = 68; y > 0; y--)
	{
		outX = (y % 2) * 8;
		int sumY = posY + y;

		if ((sumY<m_mapHeight) && (sumY >= 0))
		{
			unsigned int * pRowData = m_map_AraeHeightObject + (m_mapHeight - sumY -1)*m_mapWidth;

			for (int x = 0; x < 60; x++)
			{
				int sumX = posX + x - y / 2;

				if ((sumX < m_mapWidth) && (sumX >= 0))
				{
					int v = *(pRowData + sumX);

					int objID = (v >> 16) & 0xFF;
					int teraH = v & 0xFF;

					if (objID != 0)
					{
						clObjectTextures::ty_TextureObject * obj = &txObjects[objID];

						if (obj->firstTexture != NULL)
						{
							clObjectTextures::ty_Objects * textOb;

							int seqCount = obj->frameCount;
							if (seqCount > 1)
							{
								textOb = obj->firstTexture + ((gameLoopPos/2 + sumX + sumY / 2) % seqCount);
							}
							else
							{
								textOb = obj->firstTexture;
							}


							//--   1 --- 4
							//--   |     |
							//--   |     |
							//--   2 --- 3

							int curX = outX + textOb->xRel;
							int curY = outY - textOb->yRel - textOb->height + teraH;
							int curW = textOb->width;
							int curH = textOb->height;

							float textX = textOb->texture_x;
							float textY = textOb->texture_y;
							float textRight = textOb->texture_r;
							float textButton = textOb->texture_b;

							////// A /////
							//-- ->1
							glTexCoord2f(textX, textY);
							glVertex2i(curX, curY);

							//-- ->2
							glTexCoord2f(textX, textButton);
							glVertex2i(curX, curY + curH);

							//-- ->3
							glTexCoord2f(textRight, textButton);
							glVertex2i(curX + curW, curY + curH);

							//-- ->4
							glTexCoord2f(textRight, textY);
							glVertex2i(curX + curW, curY);



						}
						else
						{
							m_error.AddError("DrawMapObject: objectID not found: %i", objID);
						}
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
enumGradient getGradientA(unsigned char H1, unsigned char H2, unsigned char H3)
{
	//--        [North]
	//--          H1 
	//-- [West]  /  \  [East]
	//--        / A  \ 
	//--       H2 --- H3   
	//--        [South]
	if ((H1 < H3) && (H3 < H2)) return enumGradient::GRADIENT_NORTH_EAST;
	if ((H3 < H2) && (H2 < H1)) return enumGradient::GRADIENT_EAST_SOUTH;
	if ((H2 < H1) && (H1 < H3)) return enumGradient::GRADIENT_WEST;

	if ((H1 < H2) && (H2 < H3)) return enumGradient::GRADIENT_WEST_NORTH;
	if ((H2 < H3) && (H3 < H1)) return enumGradient::GRADIENT_SOUTH_WEST;
	if ((H3 < H1) && (H1 < H2)) return enumGradient::GRADIENT_EAST;
	


	if ((H2 == H3) && (H1 > H2)) return enumGradient::GRADIENT_SOUTH;
	if ((H2 == H3) && (H1 < H2)) return enumGradient::GRADIENT_NORTH;
	if ((H2 == H1) && (H3 > H1)) return enumGradient::GRADIENT_WEST_NORTH;
	if ((H2 == H1) && (H3 < H1)) return enumGradient::GRADIENT_EAST_SOUTH;
	if ((H1 == H3) && (H2 > H1)) return enumGradient::GRADIENT_NORTH_EAST;
	if ((H1 == H3) && (H2 < H1)) return enumGradient::GRADIENT_SOUTH_WEST;
	if ((H1 == H2) && (H2 == H3))return enumGradient::GRADIENT_NONE;

	return enumGradient::GRADIENT_NONE;
}


//-------------------------------------//
enumGradient getGradientB(unsigned char H1, unsigned char H2, unsigned char H3)
{
	//--        [North]
	//--       H1 --- H3 
	//--         \ B  /    
	//--  [West]  \  /  [East]
	//--           H2  
	//--        [South]
	if ((H1 < H3) && (H3 < H2)) return enumGradient::GRADIENT_WEST_NORTH;
	if ((H3 < H2) && (H2 < H1)) return enumGradient::GRADIENT_EAST;
	if ((H2 < H1) && (H1 < H3)) return enumGradient::GRADIENT_SOUTH_WEST;

	if ((H1 < H2) && (H2 < H3)) return enumGradient::GRADIENT_WEST;
	if ((H2 < H3) && (H3 < H1)) return enumGradient::GRADIENT_EAST_SOUTH;
	if ((H3 < H1) && (H1 < H2)) return enumGradient::GRADIENT_NORTH_EAST;

	if ((H1 == H3) && (H1 > H2)) return enumGradient::GRADIENT_SOUTH;
	if ((H1 == H3) && (H1 < H2)) return enumGradient::GRADIENT_NORTH;
	if ((H2 == H3) && (H3 > H1)) return enumGradient::GRADIENT_WEST_NORTH;
	if ((H2 == H3) && (H3 < H1)) return enumGradient::GRADIENT_EAST_SOUTH;
	if ((H1 == H2) && (H1 > H3)) return enumGradient::GRADIENT_NORTH_EAST;
	if ((H1 == H2) && (H1 < H3)) return enumGradient::GRADIENT_SOUTH_WEST;
	if ((H1 == H2) && (H2 == H3))return enumGradient::GRADIENT_NONE;

	return enumGradient::GRADIENT_NONE;
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
		delete[] m_map_AraeHeightObject;
		m_map_AraeHeightObject = NULL;
	}
	if (m_map_AccessiblePlayerResources != NULL)
	{
		delete[] m_map_AccessiblePlayerResources;
		m_map_AccessiblePlayerResources = NULL;
	}
	if (m_map_landscape != NULL)
	{
		delete [] m_map_landscape;
		m_map_landscape = NULL;
	}

	int bufferSize = m_mapWidth*m_mapHeight;

	//- create new map buffer
	//- MapInfos: AreaType, Height, Object
	m_map_AraeHeightObject = new unsigned int[bufferSize];

	//- MapInfo: Accessibilety, Player, Resources
	m_map_AccessiblePlayerResources = new unsigned int[bufferSize];
	

	//- read map informations
	map.readMapArea(m_map_AraeHeightObject, bufferSize, m_map_AccessiblePlayerResources, bufferSize);


	//- MapInfo structure
	m_map_landscape = new ty_mapLandscape[bufferSize];

	ty_mapLandscape * pMapPos = &m_map_landscape[0];
	unsigned int * pNeighbourN4 = &m_map_AraeHeightObject[0] - m_mapWidth + 1;
	unsigned int * pNeighbourN3 = &m_map_AraeHeightObject[0] + 1;
	
	float texWidth = 1.0f / m_LandscapeText->getWidth();
	float texHeight = 1.0f / m_LandscapeText->getHeight();

	

	//- Load the coordinates of all landscape triangles
	int count = m_LandscapeText->getTriangleTextureIDCount();
	m_texturePos_Size = count;
	m_texturePos = new ty_textureMapPos[count];

	for (int i = 0; i < count; i++)
	{
		clLandscapeTextures::tyTriangleTexture * triInfo = m_LandscapeText->getTriangleTextureInformation(i);

		m_texturePos[i].textureA_var1_X = texWidth * triInfo->up_var1_x;
		m_texturePos[i].textureA_var1_Y = texHeight * triInfo->up_var1_y;

		m_texturePos[i].textureA_var2_X = texWidth * triInfo->up_var2_x;
		m_texturePos[i].textureA_var2_Y = texHeight * triInfo->up_var2_y;


		m_texturePos[i].textureB_var1_X = texWidth * triInfo->down_var1_x;
		m_texturePos[i].textureB_var1_Y = texHeight * triInfo->down_var1_y;

		m_texturePos[i].textureB_var2_X = texWidth * triInfo->down_var2_x;
		m_texturePos[i].textureB_var2_Y = texHeight * triInfo->down_var2_y;


		m_texturePos[i].textureType = triInfo->texType;
	}




	//- Area Type
	unsigned char N1 = 7;
	unsigned char N2 = 7;
	unsigned char N3 = 7;
	unsigned char N4 = 7;

	//- Area Height
	unsigned char H1 = 0;
	unsigned char H2 = 0;
	unsigned char H3 = 0;
	unsigned char H4 = 0;

	for (int i = 0; i < bufferSize; i++)
	{
		//- ignore first row
		if (i > m_mapWidth)
		{
			unsigned int v4 = *pNeighbourN4;
			unsigned int v3 = *pNeighbourN3;
			N1 = N4;
			N2 = N3;
			N4 = (v4 >> 8) & 0xFF;
			N3 = (v3 >> 8) & 0xFF;

			H1 = H4;
			H2 = H3;
			H4 = v4 & 0xFF;
			H3 = v3 & 0xFF;
		}

		//--      N1 --- N4 
		//--     /  \ B  /    
		//--    / A  \  /    
		//--   N2 --- N3    

		pMapPos->AraeHeight2 = H2;
		pMapPos->AraeHeight1 = H1;
		pMapPos->AraeHeight3 = H3;
		pMapPos->AraeHeight4 = H4;

		
		//- get gradient of the triangles
		pMapPos->gradientA2 = getGradientA(H1, H2, H3);
		//pMapPos->gradientB = getGradientB(H1, H3, H4);


		int textureId = m_LandscapeText->getTriangleTextureID(N1, N2, N3);

		pMapPos->AraeType = N1;
		
		if (textureId >= 0)
		{
			pMapPos->textureMapPosA = textureId;
		}
		else
		{
			//- this combination is not defined
			pMapPos->textureMapPosA = m_ErrorTextureTypeID;
			
			m_error.AddError("map texture not found: %i %i %i", (int) N1, (int) N2, (int) N3);
		}

		textureId = m_LandscapeText->getTriangleTextureID(N1, N3, N4);

		if (textureId >= 0)
		{
			pMapPos->textureMapPosB = textureId;
		}
		else
		{
			//- this combination is not defined
			pMapPos->textureMapPosB = m_ErrorTextureTypeID;

			m_error.AddError("map texture not found: %i %i %i", (int) N1, (int) N3, (int) N4);
		}

		
		pNeighbourN4++;
		pNeighbourN3++;
		pMapPos++;

	}


	//----------------------------------//
	//- cache gradient for every map point
	pMapPos = &m_map_landscape[0];
	ty_mapLandscape * pMapPosN4 = &m_map_landscape[0] - m_mapWidth + 1;
	ty_mapLandscape * pMapPosN3 = &m_map_landscape[0] + 1;

	N1 = N2 = N3 = N4 = 0;

	for (int i = 0; i < bufferSize-1; i++)
	{
		//- ignore first row
		if (i > m_mapWidth)
		{
			N1 = N4;
			N2 = N3;
			N4 = pMapPosN4->gradientA2; 
			N3 = pMapPosN3->gradientA2; //- remember: is one step befor [pMapPos]

		}

		pMapPos->gradientA1 = N1;
		pMapPos->gradientA2 = N2;
		pMapPos->gradientA3 = N3;
		pMapPos->gradientA4 = N4;

		pMapPosN4++;
		pMapPosN3++;
		pMapPos++;

	}
	

}


//-------------------------------------//
void loadShader()
{
	const char* map_shader_source = STRINGIFY(
		uniform sampler2D tex;
		void main()
		{
			gl_FragColor = texture2D(tex, vec2(gl_TexCoord[0]))*gl_Color*2.0;
		}
	);


	m_map_shader.addFragmentShader(map_shader_source);
}



/*
//-------------------------------------//
void initShader()
{


	//- Create a shader program
	m_shader_Program = glCreateProgram();



	m_pixle_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_pixle_shader, 1, &pixle_shader_source, 0);
	glCompileShader(m_pixle_shader);
	//clTexturesLoadHelper::checkForGlError("glCompileShader(m_pixle_shader)");
	clTexturesLoadHelper::checkForGlShaderError(m_pixle_shader, "glCompileShader(m_pixle_shader)");


	glAttachShader(m_shader_Program, m_pixle_shader);
	clTexturesLoadHelper::checkForGlError("glAttachShader(m_shader_Program, m_pixle_shader)");

	

	//glShaderSource(vs, 1, &v, 0);
	//vs = glCreateShader(GL_VERTEX_SHADER);
	//glCompileShader(vs);
	//glAttachShader(prog, vs);

	glLinkProgram(m_shader_Program);
	//clTexturesLoadHelper::checkForGlError("glLinkProgram(m_shader_Program)");
	clTexturesLoadHelper::checkForGlShaderError(m_pixle_shader, "glAttachShader(m_shader_Program, m_pixle_shader)");


	m_brightness = glGetUniformLocation(m_shader_Program, "r_brightness");
	clTexturesLoadHelper::checkForGlError("glGetUniformLocation(r_brightness)");
}
*/

//-------------------------------------//
void loadResource()
{
	//---------------------
	//- load Landscape
	clGFXFile gfxLand = clGFXFile("Siedler3_00.f8007e01f.dat");

	//- create textur-Atlas and triangle-information-buffer
	m_LandscapeText = new clLandscapeTextures(544, 576, 300);

	//- load map patterns and define triangles
	clTexturesLoadHelper::load_map_patterns(m_LandscapeText, &gfxLand);


	//----
	//- degugging Color-patterns

	m_ErrorTextureTypeID = m_LandscapeText->AddTexturePlainColored32x32(255, 0, 0);
	m_MarkerTextureTypeID = m_LandscapeText->AddTexturePlainColored32x32(0, 0, 0);

	//- END degugging Colors



	


	//---------------------
	//- load map Objects
	clGFXFile gfxObjects = clGFXFile("Siedler3_01.f8007e01f.dat");
	

	m_ObjectText = new clObjectTextures(128 * 16, 128 * 12, 400);

	clTexturesLoadHelper::load_game_objects(m_ObjectText, &gfxObjects);

	txObjects = m_ObjectText->getObjectTexturs();


	//---------------------
	//- load buildings

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
// OPEN-GL [glfw] CALLBACKS
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
			case 'h':
			case 'H':
				m_useHeight = !m_useHeight;
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
			m_MouseDownStartX = (int)mouseX - m_mapPosX;
			m_MouseDownStartY = m_mapPosY - m_mapHeight + (int) mouseY;

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
		m_mapPosX = (int)xpos - m_MouseDownStartX;
		m_mapPosY = m_mapHeight - ((int)ypos - m_MouseDownStartY);


		if (m_mapPosX < -1) m_mapPosX = -1;
		if (m_mapPosY < -1) m_mapPosY = -1;

		if (m_mapPosX >= m_mapWidth) m_mapPosX = m_mapWidth-1;
		if (m_mapPosY >= m_mapHeight) m_mapPosY = m_mapHeight-1;
	}
}

//-------------------------------------//
static void error_callback(int error, const char* description)
{
	m_error.AddError("GLFW: [%i] %s", error, description);
}


