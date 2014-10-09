#ifndef CLTEXTURE_ATLAS_H
#define CLTEXTURE_ATLAS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "clError.h"
#include "clGFXFile.h"


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
			TEXTURE_HEIGHT_128,
			TEXTURE_HEIGHT_256,
			_TEXTURE_HEIGHT_COUNT
		};


		clTextureAtlas(int width, int height);
		~clTextureAtlas();


		void AddTexture(tyTextureAtlasPos *destPos, unsigned int * data, int width, int height, int repeatWidth = 0);
		GLuint createGLTextureAtlas();

		void loadLandscapeTextureFromGFX(tyTextureAtlasPos * destPos, clGFXFile * gfxFileObj, int gfxTextureId);


	private:
		int m_slot_pos_x[_TEXTURE_HEIGHT_COUNT];
		int m_slot_pos_y[_TEXTURE_HEIGHT_COUNT];
		unsigned int * m_buffer;
		int m_width;
		int m_height;
		int m_filled_y_pos;
		clError m_error = clError("clTextureAtlas");

		static const int SLOT_HEIGHT[];

	private:
		enumTexturSizeSlot getSlotByHeight(int height);
		void copyimage(unsigned int *dest, unsigned int * src, int posX, int posY, int width, int height, int destWidth, int srcWidth=-1);
		bool clTextureAtlas::checkForGlError(const char * errorText);
};

__declspec(selectany)
const int clTextureAtlas::SLOT_HEIGHT [] = { 8, 16, 32, 64, 128, 256 };

#endif