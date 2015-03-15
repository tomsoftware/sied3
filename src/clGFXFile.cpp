#include "clGFXFile.h"



#define max(x,y)  (((x)>(y))?(x):(y))
#define min(x,y)  (((x)<(y))?(x):(y))

//- scale Color from RGB555 to RGB888
//- 255 / 248 = 1.02822580645
//#define RGB555_TO_RGB888_FAKTOR 1.02822580645
#define RGB555_TO_RGB888_FAKTOR 1.03

//- 255/252 = 1.0119047619
#define RGB565_TO_RGB888_FAKTOR 1.012


//-------------------------------------//
clGFXFile::clGFXFile(const char * fileName)
{
	for (int i = 0; i < enum_GFX_Type::GFX_Type____COUNT; i++)
	{
		m_gfxObjects[i].objects = NULL;
		m_gfxObjects[i].count = 0;
	}

	if (fileName != NULL) openGFXFile(fileName);
}



//-------------------------------------//
void clGFXFile::cleanUp()
{
	//- read index Image-Object-Index-Table Header
	for (int i = 0; i < enum_GFX_Type::GFX_Type____COUNT; i++)
	{
		if (m_gfxObjects[i].objects != NULL)
		{
			int count = m_gfxObjects[i].count;

			//- delete sequenzes
			for (int j = 0; j < count; j++)
			{
				if (m_gfxObjects[i].objects[j].imgOffset != NULL)
				{
					delete (m_gfxObjects[i].objects[j].imgOffset);
					m_gfxObjects[i].objects[j].imgCount = 0;
					m_gfxObjects[i].objects[j].imgOffset = NULL;
				}
			}

			//- delete object
			delete(m_gfxObjects[i].objects);
			m_gfxObjects[i].objects = NULL;
		}
	}
}



//-------------------------------------//
bool clGFXFile::openGFXFile(const char * fileName)
{
	m_filename = fileName;
	m_F.readFromFile(clConfig::getPath(clConfig::CONFIG_GFX_Path), fileName);
	m_FR = m_F.getFileReader();

	//- clear buffer
	cleanUp();


	if (!m_FR->eof())
	{
		int magic = m_FR->readIntBE();
		if (magic != 0x00041304)
		{
			m_error.AddError("Not a Siedler3 GFX File: %s", fileName);
			return false;
		}

		//- @offset:33 - Color depth
		enum_COLOR_Type bit_depth = (enum_COLOR_Type) m_FR->readIntBE(1, 33);
		if (( bit_depth != enum_COLOR_Type::COLOR_BIT_DEPTH_555) && (bit_depth != enum_COLOR_Type::COLOR_BIT_DEPTH_565))
		{
			m_error.AddError("GFX File value for bit depth is unknown: %i", bit_depth);
			return false;
		}
		m_color_bit_depth = bit_depth;


		//- @offset:48 - Filesize of this File
		int fs_lenght = m_FR->readIntBE(4,48);
		if (fs_lenght != m_FR->length())
		{
			m_error.AddError("GFX File size mismatch: %s", fileName);
			return false;
		}

		//- read offsets to Image-Object Index Table
		for (int i = 0; i < enum_GFX_Type::GFX_Type____COUNT; i++)
		{
			m_gfxObjects[i].offset = m_FR->readIntBE(4);
		}

		//- read index Image-Object-Index-Table Header
		for (int i = 0; i < enum_GFX_Type::GFX_Type____COUNT; i++)
		{
			m_FR->setOffset(m_gfxObjects[i].offset);

			m_gfxObjects[i].word1 = m_FR->readIntBE(2); //- maybe the Type of this Object?
			m_gfxObjects[i].word2 = m_FR->readIntBE(2); //- everytime 0
			m_gfxObjects[i].HeaderSize = m_FR->readIntBE(2);
			int count = m_FR->readIntBE(2); //- count of gfx Oojects of this Type

			//- assign array for offsets
			if (count>0)
			{
				m_gfxObjects[i].objects = new ty_gfxSequenz[count];
			}
			else
			{
				m_gfxObjects[i].objects = NULL;
				count = 0;
			}
			m_gfxObjects[i].count = count;

			if (count>0) m_error.AddDebug("GFX Object[%s] count = %i", clGFXFile::GFX_TYPE_NAMES[i], count);

			//- special rules for some types?
			if ((i == enum_GFX_Type::GFX_Type_Pallet) && (m_gfxObjects[i].count > 0))
			{
				//- don't know why
				m_gfxObjects[i].word3 = m_FR->readIntBE(2);
				m_gfxObjects[i].word4 = m_FR->readIntBE(2);
			}


			//- read Index of all GFX-Objects (this can by Images or Sequences of Images!)
			for (int j = 0; j < count; j++)
			{
				m_gfxObjects[i].objects[j].HaedOffset = m_FR->readIntBE(4);
				m_gfxObjects[i].objects[j].imgCount = 0;
				m_gfxObjects[i].objects[j].imgOffset = NULL;
			}


			//- Some Objects are organizized into Sequenzes - read them
			if ((i == enum_GFX_Type::GFX_Type_Object) || (i == enum_GFX_Type::GFX_Type_Shadow) || (i == enum_GFX_Type::GFX_Type_Torso))
			{
				for (int j = 0; j < count; j++)
				{
					readSequenz((enum_GFX_Type) i, j);
				}
			}
			
		}
	}

	return true;
}


//-------------------------------------//
int clGFXFile::readSequenz(enum_GFX_Type GFX_Type, int sequenzeId)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[GFX_Type];

	if ((sequenzeId >= 0) && (sequenzeId < gfxOb->count))
	{
		ty_gfxSequenz * gfxSeq = &gfxOb->objects[sequenzeId];

		int offset = gfxSeq->HaedOffset;
		m_FR->setOffset(offset);

		int SequenzCode = m_FR->readIntBE(2);
		int SequenzWord1 = m_FR->readIntBE(2);
		int SequenzHeaderSize = m_FR->readIntBE(2); //- maybe the pallet-variation? ... but index goes from 0..7 ?
		int SequenzWord2 = m_FR->readIntBE(1);

		if ((SequenzCode != 0x1402) || (SequenzWord1 != 0) || (SequenzHeaderSize != 8) || (SequenzWord2 != 0))
		{
			m_error.AddError("(readSequenz) GFX_type: %s , id: %i : Wrong Sequenz Header", clGFXFile::GFX_TYPE_NAMES[GFX_Type], sequenzeId);
			return -1;
		}

		int count = m_FR->readIntBE(1);
		gfxSeq->imgCount = count;
		gfxSeq->imgOffset = new int[count];

		for (int i = 0; i < count; i++)
		{
			gfxSeq->imgOffset[i] = offset + m_FR->readIntBE(4);
		}

		return count;
	}
	else
	{
		m_error.AddError("(readSequenz) GFX_type: %s , ID %i not found!", clGFXFile::GFX_TYPE_NAMES[GFX_Type], sequenzeId);
	}

	return 0;

}


//-------------------------------------//
int clGFXFile::getAnimationInfoFrameCount(int id)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_AnimationInfo];
	if ((id < 0) || (id >= gfxOb->count)) return 0;
	
	return m_FR->readIntBE(4, gfxOb->objects[id].HaedOffset);
}


//-------------------------------------//
bool clGFXFile::getAnimationInfo(GFX_ObjectAnimationFrame * outGFXObject, int id, int frame)
{
	if (outGFXObject == NULL) return false;

	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_AnimationInfo];
	if ((id < 0) || (id >= gfxOb->count))
	{
		outGFXObject->object_id = -1;
		outGFXObject->torso_id = -1;
		outGFXObject->shadow_id = -1;

		m_error.AddError("(getAnimationInfos) Animation ID (%i) not found", id);
		return false;
	}

	int offset = gfxOb->objects[id].HaedOffset;
	m_FR->setOffset(offset);

	int frameCount = m_FR->readIntBE();

	if ((frame < 0) || (frame >= frameCount))
	{
		outGFXObject->object_id = -1;
		outGFXObject->torso_id = -1;
		outGFXObject->shadow_id = -1;

		m_error.AddError("(getAnimationInfos) Frame %i (max: %i) in Animation ID (%i) not found", frame, frameCount, id);
		return false;
	}

	//- frames are in reverse order 
	frame = frameCount - frame-1;

	//- jump to Animation frame information
	m_FR->setOffset(offset + 4 + 24 * frame);

	outGFXObject->posX =  m_FR->readSignedWordBE();
	outGFXObject->posY = m_FR->readSignedWordBE();

	outGFXObject->object_id = m_FR->readIntBE(2);
	outGFXObject->object_file = m_FR->readIntBE(2);

	outGFXObject->torso_id = m_FR->readIntBE(2);
	outGFXObject->torso_file = m_FR->readIntBE(2);

	outGFXObject->shadow_id = m_FR->readIntBE(2);
	outGFXObject->shadow_file = m_FR->readIntBE(2);

	outGFXObject->object_frame = m_FR->readIntBE(2);
	outGFXObject->torso_frame = m_FR->readIntBE(2);

	outGFXObject->sound_flag1 = m_FR->readSignedWordBE();
	outGFXObject->sound_flag2 = m_FR->readSignedWordBE();

	return true;
}


//-------------------------------------//
bool clGFXFile::getTextureLandscape(GFXFILE_TextureObject *outGFXObject, int id)
{

	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Landscape];

	if ((id < 0) || (id >= gfxOb->count))
	{
		outGFXObject->xRel = 0;
		outGFXObject->yRel = 0;
		outGFXObject->height = 0;
		outGFXObject->width = 0;
		outGFXObject->imageRGBA = NULL;

		m_error.AddError("(getTextureLandscape) Texture ID (%i) not found", id);

		return false;
	}


	//- no sequenz so read image header...
	ty_ImageHead imgHead;

	//- read Haeder of Image
	readImageHeader(&imgHead, gfxOb->objects[id].HaedOffset);


	//- get buffer for new pixel data
	unsigned int *imgData = new unsigned int[imgHead.width * imgHead.height];

	//- read Pixel Data of Image
	readImageData(imgData, imgHead.fileOffset, imgHead.width, imgHead.height, 0, 0, imgHead.width);


	outGFXObject->imageRGBA = imgData;
	outGFXObject->xRel = imgHead.xRel;
	outGFXObject->yRel = imgHead.yRel;
	outGFXObject->height = imgHead.height;
	outGFXObject->width = imgHead.width;

	return true;
}



/*
//-------------------------------------//
bool clGFXFile::getTextureTorso(GFXFILE_TextureObject *outGFXObject, SDL_Renderer* renderer, int id, int frame)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Torso];

	if ((id >= 0) && (id< gfxOb->count))
	{
		if ((frame >= 0) && (frame < gfxOb->objects[id].imgCount))
		{
			ty_ImageHead imgHead;

			//- read Haeder of Image
			readImageHeader(&imgHead, gfxOb->objects[id].imgOffset[frame], true, true, false);


			SDL_Surface* newSurface = SDL_CreateRGBSurface(0, imgHead.width, imgHead.height, 8, 0, 0, 0, 0);
			if (newSurface == 0)
			{
				m_error.AddError("(getTextureTorso) SDL_CreateRGBSurface error: %s", SDL_GetError());
				return NULL;
			}


			//- clear Buffer
			SDL_FillRect(newSurface, NULL, 0x00000000);


			SDL_LockSurface(newSurface);
			//- read Pixel Data of Index Image
			readIndexData((Uint8*) (newSurface->pixels), imgHead.fileOffset, imgHead.width, imgHead.height, 0, 0, newSurface->pitch);


			SDL_UnlockSurface(newSurface);


			outGFXObject->image = newSurface;
			outGFXObject->xRel = imgHead.xRel;
			outGFXObject->yRel = imgHead.yRel;
			outGFXObject->height = imgHead.height;
			outGFXObject->width = imgHead.width;
			return true;
		}
		else
		{
			m_error.AddError("(getTextureTorso) Frame (%i) of Texture-ID (%i) not found", frame, id);
		}
	}
	else
	{
		m_error.AddError("(getTextureTorso) Texture ID (%i) not found", id);
	}

	
	return false;
}
*/

/*
//-------------------------------------//
SDL_Palette* clGFXFile::getPalette(int paletteId, int colorVariationIndex)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Pallet];

	if ((paletteId >= 0) && (paletteId < gfxOb->count) && (colorVariationIndex >= 0) && (colorVariationIndex<8))
	{
		m_FR->setOffset(gfxOb->objects[paletteId].HaedOffset + colorVariationIndex * 256 * 2);

		SDL_Palette * newPalette = SDL_AllocPalette(256);

		for (int i = 0; i < 256; i++)
		{
			int v = m_FR->readIntBE(2);

			SDL_Color color;


			if (m_color_bit_depth == enum_COLOR_Type::COLOR_BIT_DEPTH_565)
			{
				//- scale colors to 255
				color.b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
				color.g = (int) (RGB565_TO_RGB888_FAKTOR * ((v >> 3) & 0xFC));
				color.r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 8) & 0xF8));
			}
			else if (m_color_bit_depth == enum_COLOR_Type::COLOR_BIT_DEPTH_555)
			{

				//- scale colors to 255
				color.b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
				color.g = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 2) & 0xF8));
				color.r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 7) & 0xF8));
			}
			else
			{
				color.b = color.r = color.g = 0;
			}

			color.a = 255;


			SDL_SetPaletteColors(newPalette, &color, i, 1);
		}

		//- first color is transparency
		SDL_Color color;
		color.b = 0;
		color.g = 0;
		color.r = 0;
		color.a = 0;

		SDL_SetPaletteColors(newPalette, &color, 0, 1);

		return newPalette;
	}
	else
	{
		m_error.AddError("(getPalette) paletteId (%i) not found", paletteId);
	}

	return NULL;
}
*/

//-------------------------------------//
void clGFXFile::unload_GFX_Object(GFXFILE_TextureObject *outGFXObject)
{
	if (outGFXObject->imageRGBA != NULL)
	{
		delete [] outGFXObject->imageRGBA;
		outGFXObject->imageRGBA = NULL;
		outGFXObject->width = 0;
		outGFXObject->height = 0;
	}
}

//-------------------------------------//
int clGFXFile::getTextureObjectCount()
{
	return m_gfxObjects[enum_GFX_Type::GFX_Type_Object].count;
}

//-------------------------------------//
int clGFXFile::getTextureObjectSequenzCount(int sequenzeId)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Object];

	if ((sequenzeId >= 0) && (sequenzeId < gfxOb->count))
	{
		return gfxOb->objects[sequenzeId].imgCount;
	}
	return 0;
}

//-------------------------------------//
bool clGFXFile::getTextureObject(GFXFILE_TextureObject *outGFXObject, int sequenzeId, int shadowId, int frame)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Object];
	ty_gfxObjects * gfxObShadow = &m_gfxObjects[enum_GFX_Type::GFX_Type_Shadow];


	if ((sequenzeId < 0) && (sequenzeId >= gfxOb->count) && (shadowId < 0) && (shadowId >= gfxObShadow->count))
	{
		outGFXObject->xRel = 0;
		outGFXObject->yRel = 0;
		outGFXObject->height = 0;
		outGFXObject->width = 0;
		outGFXObject->imageRGBA = NULL;

		m_error.AddError("(getTextureObject) Texture ID (%i) not found", sequenzeId);

		return false;
	}


	if ((frame >= 0) && (frame < gfxOb->objects[sequenzeId].imgCount) && (frame < gfxObShadow->objects[shadowId].imgCount))
	{
		ty_ImageHead imgHead;
		ty_ImageHead imgHeadShadow;

		//- read Haeder of Image
		readImageHeader(&imgHead, gfxOb->objects[sequenzeId].imgOffset[frame], true, true);

		//- read Haeder of Shadow
		readImageHeader(&imgHeadShadow, gfxObShadow->objects[shadowId].imgOffset[frame], true, true);
			
		//- calc bounding box for Image + Shadow
		int xRel = min(imgHead.xRel, imgHeadShadow.xRel);
		int yRel = min(imgHead.yRel, imgHeadShadow.yRel);
		int width = max((imgHead.width + imgHead.xRel), (imgHeadShadow.width + imgHeadShadow.xRel)) - xRel;
		int height = max((imgHead.height + imgHead.yRel), (imgHeadShadow.height + imgHeadShadow.yRel)) - yRel;



		//- create buffer for new pixel data
		unsigned int *imgData = new unsigned int[width * height];

		//- clear Buffer
		readImageDataClear(imgData, width, height);

		//- read Pixel Data of Shadow
		readShadowData(imgData, imgHeadShadow.fileOffset, imgHeadShadow.width, imgHeadShadow.height, imgHeadShadow.xRel - xRel, imgHeadShadow.yRel - yRel, width);

		//- read Pixel Data of Image
		readImageData(imgData, imgHead.fileOffset, imgHead.width, imgHead.height, imgHead.xRel - xRel, imgHead.yRel - yRel, width);



		outGFXObject->imageRGBA = imgData;
		outGFXObject->xRel = xRel;
		outGFXObject->yRel = yRel;
		outGFXObject->height = height;
		outGFXObject->width = width;
		return true;
	}
	else
	{
		m_error.AddError("(getTextureObject) Frame (%i) of Texture-ID (%i) not found", frame, sequenzeId);
	}



	return false;
}



//-------------------------------------//
bool clGFXFile::readImageData(unsigned int * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth)
{
	if (outImgData == NULL) return false;

	m_FR->setOffset(fileOffset);

	int y = 0;
	int x = 0; //- only for debuging!
	unsigned int * pOutStart = outImgData + y0*outImgWidth;
	unsigned int * pOut = pOutStart + x0;


	while (y < height)
	{
		int pixelCount = m_FR->readIntBE(1);
		int pixelJump = m_FR->readIntBE(1);

		//- 7bit indicate the count of transparency - bit 8 indicates the end of line
		x += (pixelJump & 0x7F);
		pOut += (pixelJump & 0x7F);

		if (pixelCount <= width - x)
		{
			for (int i = 0; i < pixelCount; i++)
			{
				int v = m_FR->readIntBE(2);

				int r;
				int g;
				int b;

				//- scale colors to 255
				if (m_color_bit_depth == enum_COLOR_Type::COLOR_BIT_DEPTH_565 )
				{
					b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
					g = (int) (RGB565_TO_RGB888_FAKTOR * ((v >> 3) & 0xFC));
					r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 8) & 0xF8));
				}
				else if (m_color_bit_depth == enum_COLOR_Type::COLOR_BIT_DEPTH_555)
				{
					b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
					g = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 2) & 0xF8));
					r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 7) & 0xF8));
					//- info: (v & 0x8000)==0
				}
				else
				{
					r = g = b = 0;
				}


				//-       Alpha                  Color
				*pOut = 0x000000FF | (r << 24) | (g << 16) | (b << 8);
				pOut++;
				x++;
			}
		}

		//- end of line
		if ((pixelJump & 0x80) > 0)
		{
			y++;
			pOut = pOutStart + y*outImgWidth + x0; //- jump to Line start
			x = 0;
		}
	}

	return true;
}


//-------------------------------------//
bool clGFXFile::readImageDataClear(unsigned int * outImgData, int width, int height)
{
	if (outImgData == NULL) return false;

	int pixelCount = width * height;

	for (int i = pixelCount; i > 0; i--)
	{
		*outImgData = 0x00000000;
		outImgData++;
	}
	return true;
}



//-------------------------------------//
bool clGFXFile::readShadowData(unsigned int * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth, bool doEdgeBlure)
{
	if (outImgData == NULL) return false;

	m_FR->setOffset(fileOffset);

	int y = 0;
	int x = 0;
	unsigned int * pOutStart = outImgData + y0*outImgWidth;
	unsigned int * pOut = pOutStart + x0;


	while (y < height)
	{
		int pixelCount = m_FR->readIntBE(1);
		int pixelJump = m_FR->readIntBE(1);

		//- 7bit indicate the count of transparency - bit 8 indicates the end of line
		x += (pixelJump & 0x7F);
		pOut += (pixelJump & 0x7F);

		if (pixelCount <= width - x)
		{
			for (int i = 0; i < pixelCount; i++)
			{
				//-       Alpha         Color
				*pOut = 0x0000007F | 0x00000000;
				pOut++;
				x++;
			}
		}

		//- end of line
		if ((pixelJump & 0x80) > 0)
		{
			y++;
			pOut = pOutStart + y*outImgWidth + x0; //- jump to Line start
			x = 0;
		}
	}

	if (doEdgeBlure)
	{
		//- blure Shadow: ToDo... make more effektive
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				pOut = pOutStart+ y*outImgWidth + x0 + x;
				if ((*pOut) != 0)
				{
					int sum = 0;
					if (x>0) if ((*(pOut - 1)) != 0) sum++;
					if (x<(width-2)) if ((*(pOut + 1)) != 0) sum++;

					if (y>0) if ((*(pOut - outImgWidth)) != 0) sum++;
					if (y<(height - 2)) if ((*(pOut + outImgWidth)) != 0) sum++;

					if (sum == 3) *pOut = 0x00000065 | 0x00000000;
					if (sum == 2) *pOut = 0x00000043 | 0x00000000;
					if (sum < 2) *pOut = 0x00000021 | 0x00000000;
				}
			}
		}
	}
	return true;
}

/*
//-------------------------------------//
bool clGFXFile::readIndexData(Uint8 * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth)
{
	if (outImgData == NULL) return false;

	m_FR->setOffset(fileOffset);

	int y = 0;
	int x = 0; //- only for debuging!
	Uint8 * pOutStart = outImgData + y0*outImgWidth;
	Uint8 * pOut = pOutStart + x0;


	while (y < height)
	{
		int pixelCount = m_FR->readIntBE(1);
		int pixelJump = m_FR->readIntBE(1);

		//- 7bit indicate the count of transparency - bit 8 indicates the end of line
		x += (pixelJump & 0x7F);
		pOut += (pixelJump & 0x7F);

		if (pixelCount <= width - x)
		{
			for (int i = 0; i < pixelCount; i++)
			{
				//- read Index
				int value = m_FR->readIntBE(1);

				//- we use the color-index 0 for transparency: this is not the way the original game handels this, but it makes it more simple for me
				*pOut = (value>0) ?value:1;
				pOut++;
				x++;
			}
		}

		//- end of line
		if ((pixelJump & 0x80) > 0)
		{
			y++;
			pOut = pOutStart + y*outImgWidth + x0; //- jump to Line start
			x = 0;
		}
	}

	return true;
}
*/

//-------------------------------------//
bool clGFXFile::readImageHeader(ty_ImageHead * imgHead, int offset, bool hasMagic, bool hasRelatives, bool hasColorMode)
{
	m_FR->setOffset(offset);

	if (hasMagic)
	{
		imgHead->DWord1 = m_FR->readIntBE(4);
		if (imgHead->DWord1 != 0xC) m_error.AddError("readImage(hasMagic) FAIL!");
	}
	else
	{
		imgHead->DWord1 = 0;
	}


	imgHead->width = m_FR->readIntBE(2);
	imgHead->height = m_FR->readIntBE(2);

	if (hasRelatives)
	{
		imgHead->xRel = m_FR->readSignedWordBE();
		imgHead->yRel = m_FR->readSignedWordBE();
	}
	else
	{
		imgHead->xRel = 0;
		imgHead->yRel = 0;
	}

	if (hasColorMode)
	{
		imgHead->colorMode = m_FR->readIntBE(1); //- ??
	}
	else
	{
		imgHead->colorMode = -1;
	}

	if ((m_FR->getOffset() % 2) == 1) m_FR->readIntBE(1);

	imgHead->fileOffset = m_FR->getOffset();


	return true;
}


//-------------------------------------//
clGFXFile::~clGFXFile()
{
	m_error.AddDebug("unload gfx file: %s", m_filename);

	cleanUp();
}


//-------------------------------------//
int clGFXFile::getTextureLandscapeCount()
{
	return m_gfxObjects[enum_GFX_Type::GFX_Type_Landscape].count;
}