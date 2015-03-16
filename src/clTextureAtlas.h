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
/// |                        |  
/// |                        |  Free space
/// +--+-----------+---------+ --
/// |44|55555555555+----+    |
/// |44|55555555555|6666|    |
/// |44|55555555555|6666|    |  Slot 2: max height: 4
/// |44|55555555555|6666|    |
/// +-----+---+----+----+-+--+ --
/// |11111+---+33333333333|  |
/// |11111|222|33333333333|  |  Slot 1: max height: 2
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

		/// <returns>return the Height of this Texture-Atlas</returns>
		int getHeight();

		/// <returns>return the Width of this Texture-Atlas</returns>
		int getWidth();




protected:
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

		/// <summary>Adds a Binary-Image from [data] to the next free Image-Slot</summary>
		/// <param name="repeatWidth">append a part of the Binary-Image [data] to the right end of the image e.g. data=12345678 repeatWidth=2 out=1234567812</param>
		/// <returns>[outTexturePos] with all information about the ney image</returns>
		void AddTextureToAtlas(tyTextureAtlasPos *destPos, unsigned int * data, int width, int height, int repeatWidth = 0);


		/// <summary>
		/// Create a Open-GL Texture out of this Atlas-Image and release the internal buffer.
		/// After this, adding a new Textures is not possible any more!
		/// Calling this Function multible times result in returning the same OpenGL Texture-ID
		/// <returns>GLuint: the OpenGL Texture-ID of the Texture</returns>
		/// </summary>
		GLuint createGLTextureAtlas();

	private:


		int m_slot_pos_x[_TEXTURE_HEIGHT_COUNT];
		int m_slot_pos_y[_TEXTURE_HEIGHT_COUNT];
		unsigned int * m_img_buffer;
		int m_width;
		int m_height;

		/// <summery>fill state in y-direction</summery>
		int m_filled_y_pos;

		/// <summary>the count of used pixels. So fill rate will be : m_fillState / (getHeight()*getWidth())</summary>
		int m_fillState = 0; //- count of used pixels

		clError m_error = clError("clTextureAtlas");
		GLuint m_GL_textureID = NULL;

		static const int SLOT_HEIGHT[];

		/// <returns>returns the Texture-Slot that can handle the given image-height</returns>
		enumTexturSizeSlot getSlotByHeight(int height);

		/// <summery>copys a part of an image from [src] to [dest] buffer.</summery>
		/// <param name="posX">X Position in [dest]</param>
		/// <param name="posY">Y Position in [dest]</param>
		/// <param name="width">copy width to [dest]</param>
		/// <param name="height">copy height to [dest]</param>
		/// <param name="destWidth">width of the [dest]-buffer itself</param>
		/// <param name="srcWidth">width of the [src]-buffer itself -- IF(srcWidth<0) THEN srcWidth=width;</param>
		void copyimage(unsigned int *dest, unsigned int * src, int posX, int posY, int width, int height, int destWidth, int srcWidth=-1);
};

__declspec(selectany)

/// <summery>predefined SLOT height</summery>
const int clTextureAtlas::SLOT_HEIGHT [] = { 8, 16, 32, 64, 85, 105, 128, 256 };

#endif