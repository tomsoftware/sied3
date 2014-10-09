#include "clTextureAtlas.h"

//-------------------------------------//
clTextureAtlas::clTextureAtlas(int width, int height)
{
	m_width = width;
	m_height = height;
	
	//- create buffer for texture-iamge
	m_buffer = new unsigned int[m_width*m_height];

	//- reset all slots
	for (int i = 0; i < _TEXTURE_HEIGHT_COUNT; i++)
	{
		m_slot_pos_x[i] = m_width; //- slot is full
		m_slot_pos_y[i] = 0; 
	}

	//- the current fill state
	m_filled_y_pos = 0;

	
}

//-------------------------------------//
clTextureAtlas::~clTextureAtlas()
{
	delete [] m_buffer;
	m_width = 0;
	m_height = 0;
}

//-------------------------------------//
clTextureAtlas::enumTexturSizeSlot clTextureAtlas::getSlotByHeight(int height)
{
	if (height <= 8) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_8;
	if (height <= 16) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_16;
	if (height <= 32) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_32;
	if (height <= 64) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_64;
	if (height <= 128) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_128;
	if (height <= 256) return clTextureAtlas::enumTexturSizeSlot::TEXTURE_HEIGHT_256;
	return clTextureAtlas::enumTexturSizeSlot::_TEXTURE_HEIGHT_COUNT;
}


//-------------------------------------//
void clTextureAtlas::AddTexture(tyTextureAtlasPos *outTexturePos, unsigned int * data, int width, int height, int repeatWidth)
{
	int widthSum = width + repeatWidth;

	if (outTexturePos != NULL)
	{
		outTexturePos->x = 0;
		outTexturePos->y = 0;
		outTexturePos->width = 0;
		outTexturePos->height = 0;
	}

	enumTexturSizeSlot slot = getSlotByHeight(height);


	if (slot == enumTexturSizeSlot::_TEXTURE_HEIGHT_COUNT)
	{
		m_error.AddError("Texture can't by coppyed to Texture Atlas! Texture height is out of range.");
		return;
	}

	if (widthSum > m_width)
	{
		m_error.AddError("Texture can't by coppyed to Texture Atlas! Texture width is to large!");
		return;
	}

	if ((m_slot_pos_x[slot] + widthSum) > m_width)
	{

		if ((m_filled_y_pos + m_slot_pos_x[slot]) > m_height)
		{
			m_error.AddError("Texture can't by coppyed to Texture Atlas! Texture Atlas is FULL!");
			return;
		}

		//- slot is full - create new!
		m_slot_pos_y[slot] = m_filled_y_pos;
		m_slot_pos_x[slot] = 0;
		m_filled_y_pos += SLOT_HEIGHT[slot];
	}

	//- copy image to slot position
	int outX = m_slot_pos_x[slot];
	int outY = m_slot_pos_y[slot];

	copyimage(m_buffer, data, outX, outY, width, height, m_width);

	//- repeat a part of the image at the end of the original image
	if (repeatWidth > 0)
	{
		copyimage(m_buffer, data, outX + width, outY, repeatWidth, height, m_width, width);
	}

	//- move slot position
	m_slot_pos_x[slot] += widthSum;


	if (outTexturePos != NULL)
	{
		outTexturePos->x = outX;
		outTexturePos->y = outY;
		outTexturePos->width = widthSum;
		outTexturePos->height = height;
	}

}


//-------------------------------------//
void clTextureAtlas::copyimage(unsigned int *dest, unsigned int * src, int posX, int posY, int width, int height, int destWidth, int srcWidth)
{
	if (srcWidth < 0) srcWidth = width;

	unsigned int * destP = dest + destWidth * posY;

	for (int y = 0; y < height; y++)
	{
		unsigned int * curDestP = destP + destWidth * y + posX;
		unsigned int * srcP = src + y * srcWidth;

		for (int x = width; x > 0; x--)
		{
			*curDestP = *srcP;
			curDestP++;
			srcP++;
		}
	}
}

//-------------------------------------//
GLuint clTextureAtlas::createGLTextureAtlas()
{
	GLuint texID = NULL;

	//- Create one OpenGL texture
	glGenTextures(1, &texID);

	//- "Bind" the newly created texture
	glBindTexture(GL_TEXTURE_2D, texID);

	//-	Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, m_buffer);
	checkForGlError("createGLTextureAtlas()");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	return texID;
}


//-------------------------------------//
bool clTextureAtlas::checkForGlError(const char * errorText)
{
	int err = glGetError();

	if (err == GL_NO_ERROR) return false;

	const char * GL_Error_text = "";
	switch (err)
	{
	case GL_INVALID_ENUM: GL_Error_text = "GL_INVALID_ENUM"; break;
	case GL_INVALID_VALUE: GL_Error_text = "GL_INVALID_VALUE"; break;
	case GL_INVALID_OPERATION: GL_Error_text = "GL_INVALID_OPERATION"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: GL_Error_text = "GL_INVALID_VALUE"; break;
	case GL_OUT_OF_MEMORY: GL_Error_text = "GL_OUT_OF_MEMORY"; break;
	case GL_STACK_UNDERFLOW: GL_Error_text = "GL_STACK_UNDERFLOW"; break;
	case GL_STACK_OVERFLOW: GL_Error_text = "GL_STACK_OVERFLOW"; break;
	}

	m_error.AddError("%s Err: %i [%s]", errorText, err, GL_Error_text);

	return true;
}
