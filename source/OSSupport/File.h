
// cFile.h

// Interfaces to the cFile class providing an OS-independent abstraction of a file.

/*
The object is optimized towards binary reads.
The object has no multithreading locks, don't use from multiple threads!
Usage:
1, Construct a cFile instance (no-param constructor)
2, Open a file using Open(), check return value for success
3, Read / write
4, Destroy the instance

-- OR --

1, Construct a cFile instance opening the file (filename-param constructor)
2, Check if the file was opened using IsOpen()
3, Read / write
4, Destroy the instance
*/





#pragma once
#ifndef CFILE_H_INCLUDED
#define CFILE_H_INCLUDED





#ifndef _WIN32
	#define USE_STDIO_FILE
#endif  // _WIN32

// DEBUG:
#define USE_STDIO_FILE





class cFile
{
public:
	#ifdef _WIN32
	static const char PathSeparator = '\\';
	#else
	static const char PathSeparator = '/';
	#endif
	
	/// The mode in which to open the file
	enum EMode
	{
		fmRead,      // Read-only. If the file doesn't exist, object will not be valid
		fmWrite,     // Write-only. If the file already exists, it will be overwritten
		fmReadWrite  // Read/write. If the file already exists, it will be left intact; writing will overwrite the data from the beginning
	} ;
	
	/// Simple constructor - creates an unopened file object, use Open() to open / create a real file
	cFile(void);
	
	/// Constructs and opens / creates the file specified, use IsOpen() to check for success
	cFile(const AString & iFileName, EMode iMode);
	
	/// Auto-closes the file, if open
	~cFile();
	
	bool Open(const AString & iFileName, EMode iMode);
	void Close(void);
	bool IsOpen(void) const;
	bool IsEOF(void) const;
	
	/// Reads  up to iNumBytes bytes into iBuffer, returns the number of bytes actually read,    or -1 on failure; asserts if not open
	int Read (void * iBuffer,       int iNumBytes);
	
	/// Writes up to iNumBytes bytes from iBuffer, returns the number of bytes actually written, or -1 on failure; asserts if not open
	int Write(const void * iBuffer, int iNumBytes);
	
	/// Seeks to iPosition bytes from file start, returns old position or -1 for failure; asserts if not open
	int Seek (int iPosition);
	
	/// Returns the current position (bytes from file start) or -1 for failure; asserts if not open
	int Tell (void) const;
	
	/// Returns the size of file, in bytes, or -1 for failure; asserts if not open
	int GetSize(void) const;
	
	/// Reads the file from current position till EOF into an AString; returns the number of bytes read or -1 for error
	int ReadRestOfFile(AString & a_Contents);
	
	/// Returns true if the file specified exists
	static bool Exists(const AString & a_FileName);
	
private:
	#ifdef USE_STDIO_FILE
	FILE * m_File;
	#else
	HANDLE m_File;
	#endif
} ;





#endif  // CFILE_H_INCLUDED




