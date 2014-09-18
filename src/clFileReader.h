#ifndef CLFILEREADER_H
#define CLFILEREADER_H

#include "clFile.h"
#include "clError.h"

class clFileReader
{

private:
	int m_offset;
	class clFile *m_clF;
	clError m_error = clError("clFileReader");
	int m_hiddenoffset;
	int m_blocklen;
	bool m_eof;



public:

	clFileReader(clFile *FileClass, int hiddenoffset = 0, int blocklen = -1);
	~clFileReader();
	bool eof();
	int length();
	int getOffset();
	bool setOffset(int newOffset);
	const char * readBuffer(int * outSize = NULL, int size = -1, int offset = -1);

	/// <summary>return a pointer to the internel byte buffer if [size] and [offset] match the size of the buffer.</summary>
	/// <param name="size">count of bytes to access.</param>
	/// <param name="offset">offset to start access.</param>
	char * DirectBufferAccess(int size, int offset=-1);

	/// <summary>return the next [size] byte and convert them to a BigEnding Integer.</summary>
	/// <param name="size">count of bytes to read.</param>
	/// <param name="offset">offset to start reading.</param>
	int readIntBE(int size = 4, int offset = -1);


	int readSignedWordBE(int offset = -1);
	
};



#endif

