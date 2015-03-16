#ifndef SCREEN_H
#define SCREEN_H

#include "openGL.h"
#include "clError.h"
#include "clObjectTextures.h"



#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/// <summary>
/// The aim of this class is to sort the textures bevor output. With [addTexture()] you can add a texture to a special position.
/// when call [render()] the textures are written to openGL starting on the top-left-point
/// </summary>
class clScreen
{
public:

	enum enumTextureObjectTypes : unsigned char
	{
		TEXTURE_OBJECTS,
		TEXTURE_BUILDINGS_ROMAN,
		//- 
		_TEXTURE_ELEMENT_COUNT_
	};



private:
	/// Y Map to pixels scale (one map map row is YSTEP pixels height)
	static const int YSTEP = (16 * 9 / 8) / 2;
	static const int XSTEP = 16;

	struct ty_point
	{
		int screen_pos_X;
		int screen_pos_Y;
		clObjectTextures::ty_Texture * texture_info;
		enumTextureObjectTypes texture_object_type;
	};

	struct ty_point * m_points;
	int m_point_max_lenght;

	int m_map_view_top;
	int m_map_view_left;
	int m_map_view_width;
	int m_map_view_height;

	clError m_error = clError("screen");

	clObjectTextures * m_texture[_TEXTURE_ELEMENT_COUNT_];

public:

	//////////////////////////////////////////////////////////////////////////////
	clScreen();

	//////////////////////////////////////////////////////////////////////////////
	~clScreen();


	//////////////////////////////////////////////////////////////////////////////
	/// <summary>Init/reset the screen class to a given map-display</summary>
	void init(int map_view_left, int map_view_top, int map_view_width, int map_view_height);

	//////////////////////////////////////////////////////////////////////////////
	/// <summary>Adds the object-texture to the screen buffer at a special pos</summary>
	/// <param name="map_pos_x">map position X of the Object</param>
	/// <param name="map_pos_y">map position Y of the Object</param>
	/// <param name="texture_map_pos_x">map position X of the Texture</param>
	/// <param name="texture_map_pos_y">map position Y of the Texture</param>
	void addTexture(int map_pos_x, int map_pos_y, int texture_map_pos_x, int texture_map_pos_y, int terrainHeight, int texture_id, enumTextureObjectTypes texture_object_type);


	//////////////////////////////////////////////////////////////////////////////
	/// <summary>return the width (in Map-Points) that can be displayed</summary>
	int clScreen::getViewMapWidth();

	//////////////////////////////////////////////////////////////////////////////
	/// <summary>return the height (in Map-Points) that can be displayed</summary>
	int clScreen::getViewMapHeight();



	//////////////////////////////////////////////////////////////////////////////
	/// <summary>writes alle Textures to the output</summary>
	void setTexture(clObjectTextures * texture, enumTextureObjectTypes textureType);

	//////////////////////////////////////////////////////////////////////////////
	/// <summary>writes alle Textures to the output</summary>
	void render();
};




#endif