
// cFile.cpp

// Implements the cFile class providing an OS-independent abstraction of a file.

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "File.h"






/// Simple constructor - creates an unopened file object, use Open() to open / create a real file
cFile::cFile(void) :
	#ifdef USE_STDIO_FILE
	m_File(NULL)
	#else
	m_File(INVALID_HANDLE_VALUE)
	#endif  // USE_STDIO_FILE
{
	// Nothing needed yet
}





/// Constructs and opens / creates the file specified, use IsOpen() to check for success
cFile::cFile(const AString & iFileName, EMode iMode) :
	#ifdef USE_STDIO_FILE
	m_File(NULL)
	#else
	m_File(INVALID_HANDLE_VALUE)
	#endif  // USE_STDIO_FILE
{
	Open(iFileName, iMode);
}





/// Auto-closes the file, if open
cFile::~cFile()
{
	if (IsOpen())
	{
		Close();
	}
}





bool cFile::Open(const AString & iFileName, EMode iMode)
{
	ASSERT(!IsOpen());  // You should close the file before opening another one
	
	if (IsOpen())
	{
		Close();
	}
	
	const char * Mode = NULL;
	switch (iMode)
	{
		case fmRead:      Mode = "rb";  break;
		case fmWrite:     Mode = "wb";  break;
		case fmReadWrite: Mode = "rb+"; break;
		default:
		{
			ASSERT(!"Unhandled file mode");
			return false;
		}
	}
	m_File = fopen( (FILE_IO_PREFIX + iFileName).c_str(), Mode);
	if ((m_File == NULL) && (iMode == fmReadWrite))
	{
		// Fix for MS not following C spec, opening "a" mode files for writing at the end only
		// The file open operation has been tried with "read update", fails if file not found
		// So now we know either the file doesn't exist or we don't have rights, no need to worry about file contents.
		// Simply re-open for read-writing, erasing existing contents:
		m_File = fopen( (FILE_IO_PREFIX + iFileName).c_str(), "wb+");
	}
	return (m_File != NULL);
}





void cFile::Close(void)
{
	ASSERT(IsOpen());  // You should not close file objects that don't have an open file.
	
	if (!IsOpen())
	{
		return;
	}

	fclose(m_File);
	m_File = NULL;
}





bool cFile::IsOpen(void) const
{
	return (m_File != NULL);
}





bool cFile::IsEOF(void) const
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		// Unopened files behave as at EOF
		return true;
	}
	
	return (feof(m_File) != 0);
}





/// Reads  up to iNumBytes bytes into iBuffer, returns the number of bytes actually read,    or -1 on failure; asserts if not open
int cFile::Read (void * iBuffer, int iNumBytes)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	return fread(iBuffer, 1, iNumBytes, m_File);  // fread() returns the portion of Count parameter actually read, so we need to send iNumBytes as Count
}





/// Writes up to iNumBytes bytes from iBuffer, returns the number of bytes actually written, or -1 on failure; asserts if not open
int cFile::Write(const void * iBuffer, int iNumBytes)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}

	int res = fwrite(iBuffer, 1, iNumBytes, m_File);  // fwrite() returns the portion of Count parameter actually written, so we need to send iNumBytes as Count
	return res;
}





/// Seeks to iPosition bytes from file start, returns old position or -1 for failure
int cFile::Seek (int iPosition)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	if (fseek(m_File, iPosition, SEEK_SET) != 0)
	{
		return -1;
	}
	return ftell(m_File);
}






/// Returns the current position (bytes from file start)
int cFile::Tell (void) const
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	return ftell(m_File);
}





/// Returns the size of file, in bytes, or -1 for failure; asserts if not open
int cFile::GetSize(void) const
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	int CurPos = ftell(m_File);
	if (CurPos < 0)
	{
		return -1;
	}
	if (fseek(m_File, 0, SEEK_END) != 0)
	{
		return -1;
	}
	int res = ftell(m_File);
	if (fseek(m_File, CurPos, SEEK_SET) != 0)
	{
		return -1;
	}
	return res;
}





int cFile::ReadRestOfFile(AString & a_Contents)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	int DataSize = GetSize() - Tell();
	
	// HACK: This depends on the internal knowledge that AString's data() function returns the internal buffer directly
	a_Contents.assign(DataSize, '\0');
	return Read((void *)a_Contents.data(), DataSize);
}





bool cFile::Exists(const AString & a_FileName)
{
	cFile test(a_FileName, fmRead);
	return test.IsOpen();
}





int cFile::Printf(const char * a_Fmt, ...)
{
	AString buf;
	va_list args;
	va_start(args, a_Fmt);
	AppendVPrintf(buf, a_Fmt, args);
	va_end(args);
	return Write(buf.c_str(), buf.length());
}




