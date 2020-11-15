
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

#include "StringUtils.h"



// tolua_begin

class cFile
{
public:

	// tolua_end
	inline static char PathSeparator()
	{
		#ifdef _WIN32
			return '\\';
		#else
			return '/';
		#endif
	}

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

	int Write(std::string_view a_String)
	{
		return Write(a_String.data(), a_String.size());
	}

	/** Seeks to iPosition bytes from file start, returns old position or -1 for failure; asserts if not open */
	long Seek (int iPosition);

	/** Returns the current position (bytes from file start) or -1 for failure; asserts if not open */
	long Tell (void) const;

	/** Returns the size of file, in bytes, or -1 for failure; asserts if not open */
	long GetSize(void) const;

	/** Reads the file from current position till EOF into an AString; returns the number of bytes read or -1 for error */
	int ReadRestOfFile(AString & a_Contents);

	/** Returns true if the file specified exists */
	static bool Exists(const AString & a_FileName);  // Exported in ManualBindings.cpp

	/** Deletes a file or a folder, returns true if successful.
	Prefer to use DeleteFile or DeleteFolder, since those don't have the penalty of checking whether a_Path is a folder. */
	static bool Delete(const AString & a_Path);  // Exported in ManualBindings.cpp

	/** Deletes a file, returns true if successful.
	Returns false if a_FileName points to a folder. */
	static bool DeleteFile(const AString & a_FileName);  // Exported in ManualBindings.cpp

	/** Deletes a folder, returns true if successful.
	Returns false if a_FolderName points to a file. */
	static bool DeleteFolder(const AString & a_FolderName);  // Exported in ManualBindings.cpp

	/** Deletes all content from the specified folder.
	The specified folder itself stays intact.
	Returns true on success, false on failure. */
	static bool DeleteFolderContents(const AString & a_FolderName);  // Exported in ManualBindings.cpp

	/** Renames a file or folder, returns true if successful. May fail if dest already exists (libc-dependant)! */
	static bool Rename(const AString & a_OrigPath, const AString & a_NewPath);  // Exported in ManualBindings.cpp

	/** Copies a file, returns true if successful.
	Overwrites the dest file if it already exists. */
	static bool Copy(const AString & a_SrcFileName, const AString & a_DstFileName);  // Exported in ManualBindings.cpp

	/** Returns true if the specified path is a folder */
	static bool IsFolder(const AString & a_Path);  // Exported in ManualBindings.cpp

	/** Returns true if the specified path is a regular file */
	static bool IsFile(const AString & a_Path);  // Exported in ManualBindings.cpp

	/** Returns the size of the file, or a negative number on error */
	static long GetSize(const AString & a_FileName);  // Exported in ManualBindings.cpp

	/** Creates a new folder with the specified name. Returns true if successful. Path may be relative or absolute */
	static bool CreateFolder(const AString & a_FolderPath);  // Exported in ManualBindings.cpp

	/** Creates a new folder with the specified name, creating its parents if needed. Path may be relative or absolute.
	Returns true if the folder exists at the end of the operation (either created, or already existed).
	Supports only paths that use the path separator used by the current platform (MSVC CRT supports slashes for file paths, too, but this function doesn't) */
	static bool CreateFolderRecursive(const AString & a_FolderPath);  // Exported in ManualBindings.cpp

	/** Returns the entire contents of the specified file as a string. Returns empty string on error. */
	static AString ReadWholeFile(const AString & a_FileName);  // Exported in ManualBindings.cpp

	/** Returns a_FileName with its extension changed to a_NewExt.
	a_FileName may contain path specification. */
	static AString ChangeFileExt(const AString & a_FileName, const AString & a_NewExt);  // Exported in ManualBindings.cpp

	/** Returns the last modification time (in current timezone) of the specified file.
	The value returned is in the same units as the value returned by time() function.
	If the file is not found / accessible, zero is returned.
	Works for folders, too, when specified without the trailing path separator. */
	static unsigned GetLastModificationTime(const AString & a_FileName);  // Exported in ManualBindings.cpp

	// tolua_begin

	/** Returns the path separator used by the current platform.
	Note that the platform / CRT may support additional path separators (such as slashes on Windows), these don't get reported. */
	static AString GetPathSeparator(void);

	/** Returns the customary executable extension used by the current platform. */
	static AString GetExecutableExt(void);

	// tolua_end

	/** Returns the list of all items in the specified folder (files, folders, nix pipes, whatever's there). */
	static AStringVector GetFolderContents(const AString & a_Folder);  // Exported in ManualBindings.cpp

	int vPrintf(const char * a_Format, fmt::printf_args a_ArgList);
	template <typename... Args>
	int Printf(const char * a_Format, const Args & ... a_Args)
	{
		return vPrintf(a_Format, fmt::make_printf_args(a_Args...));
	}

	/** Flushes all the bufferef output into the file (only when writing) */
	void Flush(void);

private:
	FILE * m_File;
} ;  // tolua_export





/** A wrapper for file streams that enables exceptions. */
template <class StreamType>
class FileStream final : public StreamType
{
public:

	FileStream(const std::string & Path)
	{
		// Except on failbit, which is what open sets on failure:
		FileStream::exceptions(FileStream::failbit | FileStream::badbit);

		// Open the file:
		FileStream::open(Path);

		// Only subsequently except on serious errors, and not on conditions like EOF or malformed input:
		FileStream::exceptions(FileStream::badbit);
	}
};





using InputFileStream = FileStream<std::ifstream>;
using OutputFileStream = FileStream<std::ofstream>;
