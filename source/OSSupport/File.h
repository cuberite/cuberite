
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





#ifndef _WIN32
	#define USE_STDIO_FILE
#endif  // _WIN32

// DEBUG:
#define USE_STDIO_FILE





// tolua_begin

class cFile
{
public:

	// tolua_end
	
	#ifdef _WIN32
	static const char PathSeparator = '\\';
	#else
	static const char PathSeparator = '/';
	#endif
	
	/// The mode in which to open the file
	enum eMode
	{
		fmRead,      // Read-only. If the file doesn't exist, object will not be valid
		fmWrite,     // Write-only. If the file already exists, it will be overwritten
		fmReadWrite  // Read/write. If the file already exists, it will be left intact; writing will overwrite the data from the beginning
	} ;
	
	/// Simple constructor - creates an unopened file object, use Open() to open / create a real file
	cFile(void);
	
	/// Constructs and opens / creates the file specified, use IsOpen() to check for success
	cFile(const AString & iFileName, eMode iMode);
	
	/// Auto-closes the file, if open
	~cFile();
	
	bool Open(const AString & iFileName, eMode iMode);
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
	
	// tolua_begin
	
	/// Returns true if the file specified exists
	static bool Exists(const AString & a_FileName);
	
	/// Deletes a file, returns true if successful
	static bool Delete(const AString & a_FileName);
	
	/// Renames a file, returns true if successful. May fail if dest already exists (libc-dependant)!
	static bool Rename(const AString & a_OrigFileName, const AString & a_NewFileName);
	
	/// Copies a file, returns true if successful.
	static bool Copy(const AString & a_SrcFileName, const AString & a_DstFileName);
	
	/// Returns true if the specified path is a folder
	static bool IsFolder(const AString & a_Path);
	
	/// Returns true if the specified path is a regular file
	static bool IsFile(const AString & a_Path);
	
	/// Returns the size of the file, or a negative number on error
	static int GetSize(const AString & a_FileName);
	
	// tolua_end
	
	int Printf(const char * a_Fmt, ...);
	
private:
	#ifdef USE_STDIO_FILE
	FILE * m_File;
	#else
	HANDLE m_File;
	#endif
} ;  // tolua_export




