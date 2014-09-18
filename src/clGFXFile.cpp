#include "clGFXFile.h"



#define max(x,y)  (((x)>(y))?(x):(y))
#define min(x,y)  (((x)<(y))?(x):(y))

//- scale Color from RGB555 to RGB888
//- 255 / 248 = 1.02822580645
//#define RGB555_TO_RGB888_FAKTOR 1.02822580645
#define RGB555_TO_RGB888_FAKTOR 1.03


clGFXFile::clGFXFile(const char * fileName)
{
	m_filename = fileName;
	m_F.readFromFile(clConfig::getPath(clConfig::CONFIG_GFX_Path), fileName);
	m_FR = m_F.getFileReader();


	if (!m_FR->eof())
	{
		int magic = m_FR->readIntBE();
		if (magic != 0x00041304)
		{
			m_error.AddError("Not a Siedler3 GFX File: %s", fileName);
			return;
		}

		//- @offset:48 - Filesize of this File
		int fs_lenght = m_FR->readIntBE(4,48);
		if (fs_lenght != m_FR->length())
		{
			m_error.AddError("GFX File size mismatch: %s", fileName);
			return;
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
bool clGFXFile::getTextureLandscape(GFX_ObjectTexture *outGFXObject, SDL_Renderer* renderer, int id)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Landscape];

	if ((id >= 0) && (id< gfxOb->count))
	{
		//- no sequenz
		ty_ImageHead imgHead;

		//- read Haeder of Image
		readImageHeader(&imgHead, gfxOb->objects[id].HaedOffset);


		SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, imgHead.width, imgHead.height);
		if (newTexture == 0)
		{
			m_error.AddError("(getTextureLandscape) SDL_CreateTexture error: %s", SDL_GetError());
			return NULL;
		}

		//- get buffer for new pixel data
		int *imgData = new int[imgHead.width * imgHead.height];

		//- read Pixel Data of Image
		readImageData(imgData, imgHead.fileOffset, imgHead.width, imgHead.height, 0, 0, imgHead.width);

		//- update Texture
		if (SDL_UpdateTexture(newTexture, NULL, imgData, imgHead.width*sizeof(int)) != 0)
		{
			delete imgData;
			m_error.AddError("(getTextureLandscape) SDL_UpdateTexture error: %s", SDL_GetError());
			return NULL;
		}

		delete imgData;

		outGFXObject->image = newTexture;
		outGFXObject->xRel = imgHead.xRel;
		outGFXObject->yRel = imgHead.yRel;
		outGFXObject->height = imgHead.height;
		outGFXObject->width = imgHead.width;
		return true;

	}
	else
	{
		m_error.AddError("(getTextureLandscape) Texture ID (%i) not found", id);
	}

	outGFXObject->xRel = 0;
	outGFXObject->yRel = 0;
	outGFXObject->height = 0;
	outGFXObject->width = 0;
	outGFXObject->image = NULL;
	return false;
}



//-------------------------------------//
bool clGFXFile::getTextureTorso(GFX_ObjectSurface *outGFXObject, SDL_Renderer* renderer, int id, int frame)
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
		m_error.AddError("(getTextureObject) Texture ID (%i) not found", id);
	}

	/*
	outGFXObject->xRel = 0;
	outGFXObject->yRel = 0;
	outGFXObject->height = 0;
	outGFXObject->width = 0;
	outGFXObject->image = NULL;
	*/
	return false;
}


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

			//- scale colors to 255
			color.b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
			color.g = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 2) & 0xF8));
			color.r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 7) & 0xF8));
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


//-------------------------------------//
bool clGFXFile::getTextureObject(GFX_ObjectTexture *outGFXObject, SDL_Renderer* renderer, int sequenzeId, int shadowId, int frame)
{
	ty_gfxObjects * gfxOb = &m_gfxObjects[enum_GFX_Type::GFX_Type_Object];
	ty_gfxObjects * gfxObShadow = &m_gfxObjects[enum_GFX_Type::GFX_Type_Shadow];

	if ((sequenzeId >= 0) && (sequenzeId< gfxOb->count) && (shadowId >= 0) && (shadowId<gfxObShadow->count))
	{
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


			SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
			if (newTexture == 0)
			{
				m_error.AddError("(getTextureObject) SDL_CreateTexture error: %s", SDL_GetError());
				return NULL;
			}

			SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

			//- create buffer for new pixel data
			int *imgData = new int[width * height];

			//- clear Buffer
			readImageDataClear(imgData, width, height);

			//- read Pixel Data of Shadow
			readShadowData(imgData, imgHeadShadow.fileOffset, imgHeadShadow.width, imgHeadShadow.height, imgHeadShadow.xRel - xRel, imgHeadShadow.yRel - yRel, width);

			//- read Pixel Data of Image
			readImageData(imgData, imgHead.fileOffset, imgHead.width, imgHead.height, imgHead.xRel - xRel, imgHead.yRel - yRel, width);


			//- update Texture
			if (SDL_UpdateTexture(newTexture, NULL, imgData, width * sizeof(int)) != 0)
			{
				delete imgData;
				m_error.AddError("(getTextureObject) SDL_UpdateTexture error: %s", SDL_GetError());
				return NULL;
			}

			delete imgData;

			outGFXObject->image = newTexture;
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
	}
	else
	{
		m_error.AddError("(getTextureObject) Texture ID (%i) not found", sequenzeId);
	}

	/*
	outGFXObject->xRel = 0;
	outGFXObject->yRel = 0;
	outGFXObject->height = 0;
	outGFXObject->width = 0;
	outGFXObject->image = NULL;
	*/
	return false;
}


//-------------------------------------//
bool clGFXFile::readImageData(int * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth)
{
	if (outImgData == NULL) return false;

	m_FR->setOffset(fileOffset);

	int y = 0;
	int x = 0; //- only for debuging!
	int * pOutStart = outImgData + y0*outImgWidth;
	int * pOut = pOutStart + x0;


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

				//- scale colors to 255
				int b = (int) (RGB555_TO_RGB888_FAKTOR * ((v << 3) & 0xF8));
				int g = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 2) & 0xF8));
				int r = (int) (RGB555_TO_RGB888_FAKTOR * ((v >> 7) & 0xF8));

				//- info: (v & 0x8000)==0

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
bool clGFXFile::readImageDataClear(int * outImgData, int width, int height)
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
bool clGFXFile::readShadowData(int * outImgData, int fileOffset, int width, int height, int x0, int y0, int outImgWidth, bool doEdgeBlure)
{
	if (outImgData == NULL) return false;

	m_FR->setOffset(fileOffset);

	int y = 0;
	int x = 0; //- only for debuging!
	int * pOutStart = outImgData + y0*outImgWidth;
	int * pOut = pOutStart + x0;


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
