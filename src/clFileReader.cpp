#include "clFileReader.h"



clFileReader::clFileReader(clFile *FileClass, int hiddenoffset, int blocklen) :
	m_clF(FileClass),
	m_offset(0),
	m_hiddenoffset(hiddenoffset),
	m_blocklen(-1),
	m_eof(true)
{

	if (blocklen>m_clF->contentLen()) blocklen = m_clF->contentLen();
	if (m_clF->contentLen()>0)
	{
		m_eof = false;
		if (blocklen >= 0)
		{
			m_blocklen = blocklen;
		}
		else
		{
			m_blocklen = m_clF->contentLen();
		}
	}
	else
	{
		m_eof = true;
		m_blocklen = 0;
	}

}


// -------------------------------------- //
bool clFileReader::eof()
{
	return m_eof;
}

// -------------------------------------- //
int clFileReader::length()
{
	return m_blocklen;
}

// -------------------------------------- //
int clFileReader::getOffset()
{
	return m_offset;
}



// -------------------------------------- //
bool clFileReader::setOffset(int newOffset)
{
	m_offset = newOffset;

	m_eof = (m_offset>m_blocklen);

	return m_eof;
}

// -------------------------------------- //
char * clFileReader::DirectBufferAccess(int size, int offset)
{
	if (size < 0)  return NULL;
	if (offset==-1) offset=m_offset;
	if (offset < 0) return NULL;
	if (m_blocklen < size + offset) return NULL;

	return (char *)m_clF->readBuffer(m_hiddenoffset + offset);
}

// -------------------------------------- //
const char * clFileReader::readBuffer(int * outSize, int size, int offset)
{
	const char * out;

	if (offset>-1)
	{
		m_offset = offset;
		m_eof = false; //- we do check this later...
	}
	
	if (size == -1) size = m_blocklen - m_offset;
	if (size < 0) size = 0;

	
	int ofs = m_hiddenoffset + m_offset;

	//- not EOF?
	if (m_offset<m_blocklen)
	{
		//- requestet size is available?
		if (m_offset + size>m_blocklen)
		{
			m_eof = true;

			if (outSize == NULL)
			{	
				m_error.AddError("Out of filesize! Available filesize: %i / requestet size: %i", m_blocklen - m_offset, size);
				size = m_blocklen - m_offset;
				return NULL; //- the required Buffer size can't be returned
			}

			m_eof = true;
			size = m_blocklen - m_offset;
		}
		out = m_clF->readBuffer(ofs);
	}
	else
	{
		m_error.AddError("EOF!");
		m_eof = true;
		size = 0;
		out = NULL;
	}
	m_offset += size;

	if (outSize != NULL) *outSize = size;

	return out;
}

// -------------------------------------- //
// - returns signed (e.g. negativ) intager values
int clFileReader::readIntBE(int size, int offset)
{
	int retSize = 0;
	const char * tmp = readBuffer(&retSize, size, offset);

	if (tmp == NULL) return 0;
	if (retSize == 4) return *((int*)tmp);
	if (retSize == 2) return ((*((int*) tmp)) & 0x0000FFFF);
	if (retSize == 1) return ((*((int*) tmp)) & 0x000000FF);
	if (retSize == 3) return ((*((int*) tmp)) & 0x00FFFFFF);

	return 0;
}

// -------------------------------------- //
int clFileReader::readSignedWordBE(int offset)
{
	int retSize = 0;
	const char * tmp = readBuffer(&retSize, 2, offset);

	if (retSize != 2) return 0;

	return (*((short*) tmp));
}

// -------------------------------------- //
clFileReader::~clFileReader()
{
	//m_error.AddDebug("~clFileReader");
}

