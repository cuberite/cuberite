
// GZipFile.cpp

// Implements the cGZipFile class representing a RAII wrapper over zlib's GZip file routines

#include "Globals.h"
#include "GZipFile.h"





cGZipFile::cGZipFile(void) :
	m_File(NULL)
{
}





cGZipFile::~cGZipFile()
{
	Close();
}





bool cGZipFile::Open(const AString & a_FileName, eMode a_Mode)
{
	if (m_File != NULL)
	{
		ASSERT(!"A file is already open in this object");
		return false;
	}
	m_File = gzopen(a_FileName.c_str(), (a_Mode == fmRead) ? "r" : "w");
	m_Mode = a_Mode;
	return (m_File != NULL);
}





void cGZipFile::Close(void)
{
	if (m_File != NULL)
	{
		gzclose(m_File);
		m_File = NULL;
	}
}





int cGZipFile::ReadRestOfFile(AString & a_Contents)
{
	if (m_File == NULL)
	{
		ASSERT(!"No file has been opened");
		return -1;
	}
	
	if (m_Mode != fmRead)
	{
		ASSERT(!"Bad file mode, cannot read");
		return -1;
	}
	
	// Since the gzip format doesn't really support getting the uncompressed length, we need to read incrementally. Yuck!
	int NumBytesRead = 0;
	int TotalBytes = 0;
	char Buffer[64 KiB];
	while ((NumBytesRead = gzread(m_File, Buffer, sizeof(Buffer))) > 0)
	{
		TotalBytes += NumBytesRead;
		a_Contents.append(Buffer, (size_t)NumBytesRead);
	}
	// NumBytesRead is < 0 on error
	return (NumBytesRead >= 0) ? TotalBytes : NumBytesRead;
}





bool cGZipFile::Write(const char * a_Contents, int a_Size)
{
	if (m_File == NULL)
	{
		ASSERT(!"No file has been opened");
		return false;
	}

	if (m_Mode != fmWrite)
	{
		ASSERT(!"Bad file mode, cannot write");
		return false;
	}

	return (gzwrite(m_File, a_Contents, (unsigned int)a_Size) != 0);
}




