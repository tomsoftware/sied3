#ifndef CLFILE_H
#define CLFILE_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "clFileReader.h"
#include "clError.h"

using namespace std;

class clFile
{
private:
	char * m_buffer;
	clError m_error = clError("clFile");
	std::string m_filename;
	unsigned int m_filesize;
	

public:
	//clFile();
	clFile::clFile();
	clFile::clFile(const char * FileName);
	clFile::clFile(const char * FilePath, const char * FileName);
	~clFile();


	/// <summary>Read data from File</summary>
	/// <param name="FilePath">Path of File to read in</param>
	/// <param name="FileName">Filename of File to read in</param>
	/// <returns>true on success</returns>
	bool readFromFile(const char * FilePath, const char * FileName);


	/// <summary>Read data from File</summary>
	/// <param name="FileName">Path+Filename of File to read in</param>
	/// <returns>true on success</returns>
	bool readFromFile(const char * FileName);


	/// <summary>Returns a new FileReaderClass of the current File</summary>
	/// <return>new clFileReader on success</return>
	class clFileReader *getFileReader();

	const char *readBuffer(int offset);

	int contentLen();
};

#endif