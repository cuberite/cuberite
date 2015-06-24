
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

For reading entire files into memory, just use the static cFile::ReadWholeFile()
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
	
	/** The mode in which to open the file */
	enum eMode
	{
		fmRead,       // Read-only. If the file doesn't exist, object will not be valid
		fmWrite,      // Write-only. If the file already exists, it will be overwritten
		fmReadWrite,  // Read / write. If the file already exists, it will be left intact; writing will overwrite the data from the beginning
		fmAppend      // Write-only. If the file already exists cursor will be moved to the end of the file
	} ;
	
	/** Simple constructor - creates an unopened file object, use Open() to open / create a real file */
	cFile(void);
	
	/** Constructs and opens / creates the file specified, use IsOpen() to check for success */
	cFile(const AString & iFileName, eMode iMode);
	
	/** Auto-closes the file, if open */
	~cFile();
	
	bool Open(const AString & iFileName, eMode iMode);
	void Close(void);
	bool IsOpen(void) const;
	bool IsEOF(void) const;
	
	/** Reads up to a_NumBytes bytes into a_Buffer, returns the number of bytes actually read, or -1 on failure; asserts if not open */
	int Read(void * a_Buffer, size_t a_NumBytes);
	
	/** Reads up to a_NumBytes bytes, returns the bytes actually read, or empty string on failure; asserts if not open */
	AString Read(size_t a_NumBytes);

	/** Writes up to a_NumBytes bytes from a_Buffer, returns the number of bytes actually written, or -1 on failure; asserts if not open */
	int Write(const void * a_Buffer, size_t a_NumBytes);
	
	/** Seeks to iPosition bytes from file start, returns old position or -1 for failure; asserts if not open */
	long Seek (int iPosition);
	
	/** Returns the current position (bytes from file start) or -1 for failure; asserts if not open */
	long Tell (void) const;
	
	/** Returns the size of file, in bytes, or -1 for failure; asserts if not open */
	long GetSize(void) const;
	
	/** Reads the file from current position till EOF into an AString; returns the number of bytes read or -1 for error */
	int ReadRestOfFile(AString & a_Contents);
	
	// tolua_begin
	
	/** Returns true if the file specified exists */
	static bool Exists(const AString & a_FileName);
	
	/** Deletes a file, returns true if successful */
	static bool Delete(const AString & a_FileName);
	
	/** Renames a file or folder, returns true if successful. May fail if dest already exists (libc-dependant)! */
	static bool Rename(const AString & a_OrigPath, const AString & a_NewPath);
	
	/** Copies a file, returns true if successful. */
	static bool Copy(const AString & a_SrcFileName, const AString & a_DstFileName);
	
	/** Returns true if the specified path is a folder */
	static bool IsFolder(const AString & a_Path);
	
	/** Returns true if the specified path is a regular file */
	static bool IsFile(const AString & a_Path);
	
	/** Returns the size of the file, or a negative number on error */
	static long GetSize(const AString & a_FileName);
	
	/** Creates a new folder with the specified name. Returns true if successful. Path may be relative or absolute */
	static bool CreateFolder(const AString & a_FolderPath);
	
	// tolua_end

	/** Returns the entire contents of the specified file as a string. Returns empty string on error.
	Exported manually in ManualBindings.cpp due to #1914 - ToLua code doesn't work well with binary files. */
	static AString ReadWholeFile(const AString & a_FileName);

	// tolua_begin

	/** Returns a_FileName with its extension changed to a_NewExt.
	a_FileName may contain path specification. */
	static AString ChangeFileExt(const AString & a_FileName, const AString & a_NewExt);

	/** Returns the last modification time (in current timezone) of the specified file.
	The value returned is in the same units as the value returned by time() function.
	If the file is not found / accessible, zero is returned. */
	static unsigned GetLastModificationTime(const AString & a_FileName);

	/** Returns the path separator used by the current platform.
	Note that the platform / CRT may support additional path separators (such as slashes on Windows), these don't get reported. */
	static AString GetPathSeparator(void);

	/** Returns the customary executable extension used by the current platform. */
	static AString GetExecutableExt(void);

	// tolua_end
	
	/** Returns the list of all items in the specified folder (files, folders, nix pipes, whatever's there). */
	static AStringVector GetFolderContents(const AString & a_Folder);  // Exported in ManualBindings.cpp

	int Printf(const char * a_Fmt, ...) FORMATSTRING(2, 3);
	
	/** Flushes all the bufferef output into the file (only when writing) */
	void Flush(void);
	
private:
	#ifdef USE_STDIO_FILE
	FILE * m_File;
	#else
	HANDLE m_File;
	#endif
} ;  // tolua_export




