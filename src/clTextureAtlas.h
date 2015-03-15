#ifndef CLTEXTURE_ATLAS_H
#define CLTEXTURE_ATLAS_H

#include "openGL.h"

#include "clError.h"
#include "clGFXFile.h"

/// <summary>
/// A Texture Atlas is a image that contains multiple Textures. 
///  - the size of the Image cant be changed after creating
///  - New textures can be added with AddTexture()
///  - use createGLTextureAtlas() to copy Image to GL-Buffer: THIS WILL DELETE THE image buffer!
/// intern: For easy handling new textures there are so called slots. Every slot has a maximum height.
///         If a new texture is added the smallest slot is selected and the new texture is copyed to
///         the right end of the slot. If no slot is available, a new one, in a free space in the image
///         will be generated
/// +--+-----------+---------+ --
/// |44|55555555555+----+    |
/// |44|555 img 555|6666|    |
/// |44|55555555555|6666|    |  Slot: max height: 4
/// |44|55555555555|6666|    |
/// +-----+---+----+----+-+  | --
/// |11111+---+33333333333|  |
/// |11111|222|33333333333|  |  Slot: max height: 2
/// +-----+---+-----------+--+ --
/// </summary>
class clTextureAtlas
{

	public:
		struct tyTextureAtlasPos
		{
			int x;
			int y;
			int width;
			int height;
		};


		enum enumTexturSizeSlot
		{
			TEXTURE_HEIGHT_8,
			TEXTURE_HEIGHT_16,
			TEXTURE_HEIGHT_32,
			TEXTURE_HEIGHT_64,
			TEXTURE_HEIGHT_85,
			TEXTURE_HEIGHT_105,
			TEXTURE_HEIGHT_128,
			TEXTURE_HEIGHT_256,
			_TEXTURE_HEIGHT_COUNT
		};


		clTextureAtlas(int width, int height);
		~clTextureAtlas();


		void AddTexture(tyTextureAtlasPos *destPos, unsigned int * data, int width, int height, int repeatWidth = 0);
		GLuint createGLTextureAtlas();


		/// <summary>
		///  the count of used pixels. So fill rate will be : m_fillState / (getHeight()*getWidth())
		/// </summary>
		int m_fillState = 0; //- count of used pixels
		

		int getHeight();
		int getWidth();

	private:


		int m_slot_pos_x[_TEXTURE_HEIGHT_COUNT];
		int m_slot_pos_y[_TEXTURE_HEIGHT_COUNT];
		unsigned int * m_img_buffer;
		int m_width;
		int m_height;
		int m_filled_y_pos;
		clError m_error = clError("clTextureAtlas");
		GLuint m_GL_textureID = NULL;

		static const int SLOT_HEIGHT[];

	private:
		enumTexturSizeSlot getSlotByHeight(int height);
		void copyimage(unsigned int *dest, unsigned int * src, int posX, int posY, int width, int height, int destWidth, int srcWidth=-1);
};

__declspec(selectany)
const int clTextureAtlas::SLOT_HEIGHT [] = { 8, 16, 32, 64, 85, 105, 128, 256 };

#endif