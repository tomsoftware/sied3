#include "clScreen.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))


//////////////////////////////////////////////////////////////////////////////
clScreen::clScreen()
{
	m_map_view_height = 0;
	m_map_view_left = 0;
	m_map_view_top = 0;
	m_map_view_width = 0;

	//- predefine to a common buffer size.
	m_point_max_lenght = 100 * 100;
	m_points = new ty_point[m_point_max_lenght];

	//- reset all [m_texture]
	for (int i = enumTextureObjectTypes::_TEXTURE_ELEMENT_COUNT_; i > 0; i--) m_texture[i] = NULL;
	
}

//////////////////////////////////////////////////////////////////////////////
clScreen::~clScreen()
{
	delete []m_points;
}


//////////////////////////////////////////////////////////////////////////////
/// <summary>Init/reset the screen class to a given map-display</summary>
void clScreen::init(int map_view_left, int map_view_top, int map_view_width, int map_view_height)
{
	if (map_view_height < 1) map_view_height = 1;
	if (map_view_width < 1) map_view_width = 1;
	if (map_view_left < 0) map_view_left = 0;
	if (map_view_top < 0) map_view_top = 0;

	m_map_view_left = map_view_left;
	m_map_view_top = map_view_top;
	m_map_view_width = map_view_width;
	m_map_view_height = map_view_height;


	//m_error.AddDebug("x: %i, y:%i", map_view_left, map_view_top);

	//- resize screen buffer
	int newLenght = m_map_view_width  * m_map_view_height;
	if (newLenght > m_point_max_lenght)
	{
		delete []m_points;
		m_point_max_lenght = newLenght + 100;
		m_points = new ty_point[m_point_max_lenght];
	}


	//- reset screen buffer
	ty_point * p = m_points;
	for (int i = newLenght; i > 0; i--)
	{
		p->texture_info = NULL;
		p++;
	}
}


//////////////////////////////////////////////////////////////////////////////
int clScreen::getViewMapWidth()
{
	return m_map_view_width;
}


//////////////////////////////////////////////////////////////////////////////
/// <summary>return the height (in Map-Points) that can be displayed</summary>
int clScreen::getViewMapHeight()
{
	return m_map_view_height;
}


//////////////////////////////////////////////////////////////////////////////
void clScreen::addTexture(int map_pos_x, int map_pos_y, int texture_map_pos_x, int texture_map_pos_y, int terrainHeight, int texture_id, enumTextureObjectTypes texture_object_type)
{
	int map_rel_y = texture_map_pos_y - m_map_view_top;
	int map_rel_x = texture_map_pos_x - m_map_view_left + (map_rel_y / 2); //- because of the "Axonometric projection" we bend our [m_points] array

	//- check if the texture is in the view?
	if ((map_rel_x < 0) || (map_rel_x >= m_map_view_width)) return; //- out of range
	if ((map_rel_y < 0) || (map_rel_y >= m_map_view_height)) return; //- out of range

	//- the texture is saved at (texture_map_pos_x / texture_map_pos_y)
	ty_point * p = &m_points[map_rel_x + (m_map_view_height-map_rel_y) * m_map_view_width];

	//- read Textur Informations
	clObjectTextures::ty_Texture * TextureInfo = m_texture[texture_object_type]->getTextures() + texture_id;


	int x = map_pos_x - m_map_view_left;
	int y = map_pos_y - m_map_view_top;


	p->screen_pos_X = ((x + y / 2)*XSTEP + (y % 2) * XSTEP/2 ) + TextureInfo->xRel;
	p->screen_pos_Y = y*YSTEP - TextureInfo->yRel - TextureInfo->height + terrainHeight; 

	p->texture_object_type = texture_object_type;
	p->texture_info = TextureInfo;
}

//////////////////////////////////////////////////////////////////////////////
/// <summary>writes alle Textures to the output</summary>
void clScreen::setTexture(clObjectTextures * texture, enumTextureObjectTypes texture_object_type)
{
	m_texture[texture_object_type] = texture;
}


//////////////////////////////////////////////////////////////////////////////
/// <summary>writes alle Textures to the output</summary>
void clScreen::render()
{
	//- range to display
	int view_width = m_map_view_width;
	int view_height = m_map_view_height;
	//int posX = m_map_view_left;
	//int posY = m_map_view_top;



	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//- get the texture ID
	enumTextureObjectTypes TextureID = enumTextureObjectTypes::_TEXTURE_ELEMENT_COUNT_;


	ty_point * p = m_points-1;

	

	//- go throut the [m_points]-Object and draw everything
	for (int i = view_height*view_width; i > 0; i--)
	{
		p++;

		if (p->texture_info != NULL)
		{
			clObjectTextures::ty_Texture * textureInfo = p->texture_info;

			if (p->texture_object_type != TextureID)
			{
				//- if this is not the first Quad then we have to colose it
				if (TextureID != enumTextureObjectTypes::_TEXTURE_ELEMENT_COUNT_) glEnd();

				TextureID = p->texture_object_type;
				glBindTexture(GL_TEXTURE_2D, m_texture[TextureID]->createGLTextureAtlas());
				glBegin(GL_QUADS);
			}
			

			//- Landscape
			//--      1 --- 4  ^
			//--     / \ B /   |
			//--    / A \ /    |YSTEP
			//--   2 --- 3     v

			//- Object
			//--   1 --- 4   ^
			//--   |     |   |
			//--   |     |   | Object/texture Height
			//--   |     |   | 
			//--   2 --- 3   v

			int curX = p->screen_pos_X;
			int curY = p->screen_pos_Y;
			int curW = textureInfo->width;
			int curH = textureInfo->height;

			float textX = textureInfo->texture_x;
			float textY = textureInfo->texture_y;
			float textRight = textureInfo->texture_r;
			float textButton = textureInfo->texture_b;

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
	}

	//- if we did at least one quadrad then has to get closed
	if (TextureID != enumTextureObjectTypes::_TEXTURE_ELEMENT_COUNT_) glEnd();

	glDisable(GL_TEXTURE_2D);
}



