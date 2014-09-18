#include "clFile.h"


clFile::clFile()
{
	m_buffer = NULL;
	m_filename = "";
	m_filesize = 0;
}

//--------------------------//
clFile::clFile(const char * FileName)
{
	m_buffer = NULL;
	m_filename = "";
	m_filesize = 0;

	readFromFile(FileName);
}


//--------------------------//
clFile::clFile(const char * FilePath, const char * FileName)
{
	m_buffer = NULL;
	m_filename = "";
	m_filesize = 0;

	readFromFile(std::string(FilePath).append(FileName).c_str());
}

//--------------------------//
bool clFile::readFromFile(const char * FilePath, const char * FileName)
{
	return readFromFile(std::string(FilePath).append(FileName).c_str());
}

//--------------------------//
bool clFile::readFromFile(const char * FileName)
{
	//- save Filename
	m_filename = "";

	//- delete buffer if already used
	if (m_buffer != NULL) delete m_buffer;

	m_filesize = 0;

	ifstream myFile(FileName, ios::in | ios::binary);
	if (myFile)
	{
		m_error.AddDebug("Open file %s", FileName);

		m_filename = FileName;

		//- get stream size
		myFile.seekg(0, myFile.end);
		m_filesize = (unsigned int)myFile.tellg();
		myFile.seekg(0, myFile.beg);

		//- read file to buffer
		m_buffer = new char[m_filesize];

		myFile.read(m_buffer, m_filesize);
	}
	else
	{
		m_error.AddError("Unable to open file: %s", FileName);
	}

	return true;
}

//--------------------------//
clFile::~clFile()
{
	if (m_buffer != NULL)
	{
		m_error.AddDebug("Close file %s", m_filename.c_str());
		delete m_buffer;
	}
}

// -------------------------------------- //
clFileReader * clFile::getFileReader()
{
	return new clFileReader(this);

}

// -------------------------------------- //
const char *clFile::readBuffer(int offset)
{
	return m_buffer + offset;
}

// -------------------------------------- //
int clFile::contentLen()
{
	return m_filesize;
}