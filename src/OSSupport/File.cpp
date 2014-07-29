
// cFile.cpp

// Implements the cFile class providing an OS-independent abstraction of a file.

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "File.h"
#include <fstream>
#ifdef _WIN32
	#include <share.h>  // for _SH_DENYWRITE
#endif  // _WIN32





cFile::cFile(void) :
	#ifdef USE_STDIO_FILE
	m_File(NULL)
	#else
	m_File(INVALID_HANDLE_VALUE)
	#endif  // USE_STDIO_FILE
{
	// Nothing needed yet
}





cFile::cFile(const AString & iFileName, eMode iMode) :
	#ifdef USE_STDIO_FILE
	m_File(NULL)
	#else
	m_File(INVALID_HANDLE_VALUE)
	#endif  // USE_STDIO_FILE
{
	Open(iFileName, iMode);
}





cFile::~cFile()
{
	if (IsOpen())
	{
		Close();
	}
}





bool cFile::Open(const AString & iFileName, eMode iMode)
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
	}
	if (Mode == NULL)
	{
		ASSERT(!"Unhandled file mode");
		return false;
	}

#ifdef _WIN32
	m_File = _fsopen((FILE_IO_PREFIX + iFileName).c_str(), Mode, _SH_DENYWR);
#else
	m_File = fopen((FILE_IO_PREFIX + iFileName).c_str(), Mode);
#endif  // _WIN32

	if ((m_File == NULL) && (iMode == fmReadWrite))
	{
		// Fix for MS not following C spec, opening "a" mode files for writing at the end only
		// The file open operation has been tried with "read update", fails if file not found
		// So now we know either the file doesn't exist or we don't have rights, no need to worry about file contents.
		// Simply re-open for read-writing, erasing existing contents:

#ifdef _WIN32
		m_File = _fsopen((FILE_IO_PREFIX + iFileName).c_str(), "wb+", _SH_DENYWR);
#else
		m_File = fopen((FILE_IO_PREFIX + iFileName).c_str(), "wb+");
#endif  // _WIN32

	}
	return (m_File != NULL);
}





void cFile::Close(void)
{
	if (!IsOpen())
	{
		// Closing an unopened file is a legal nop
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





int cFile::Read (void * iBuffer, size_t iNumBytes)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	return (int)fread(iBuffer, 1, (size_t)iNumBytes, m_File);  // fread() returns the portion of Count parameter actually read, so we need to send iNumBytes as Count
}





int cFile::Write(const void * iBuffer, size_t iNumBytes)
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}

	int res = (int)fwrite(iBuffer, 1, (size_t)iNumBytes, m_File);  // fwrite() returns the portion of Count parameter actually written, so we need to send iNumBytes as Count
	return res;
}





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
	return (int)ftell(m_File);
}






int cFile::Tell (void) const
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	return (int)ftell(m_File);
}





int cFile::GetSize(void) const
{
	ASSERT(IsOpen());
	
	if (!IsOpen())
	{
		return -1;
	}
	
	int CurPos = Tell();
	if (CurPos < 0)
	{
		return -1;
	}
	if (fseek(m_File, 0, SEEK_END) != 0)
	{
		return -1;
	}
	int res = Tell();
	if (fseek(m_File, (long)CurPos, SEEK_SET) != 0)
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
	a_Contents.assign((size_t)DataSize, '\0');
	return Read((void *)a_Contents.data(), DataSize);
}





bool cFile::Exists(const AString & a_FileName)
{
	cFile test(a_FileName, fmRead);
	return test.IsOpen();
}





bool cFile::Delete(const AString & a_FileName)
{
	return (remove(a_FileName.c_str()) == 0);
}





bool cFile::Rename(const AString & a_OrigFileName, const AString & a_NewFileName)
{
	return (rename(a_OrigFileName.c_str(), a_NewFileName.c_str()) == 0);
}





bool cFile::Copy(const AString & a_SrcFileName, const AString & a_DstFileName)
{
	#ifdef _WIN32
		return (CopyFile(a_SrcFileName.c_str(), a_DstFileName.c_str(), true) != 0);
	#else
		// Other OSs don't have a direct CopyFile equivalent, do it the harder way:
		std::ifstream src(a_SrcFileName.c_str(), std::ios::binary);
		std::ofstream dst(a_DstFileName.c_str(), std::ios::binary);
		if (dst.good())
		{
			dst << src.rdbuf();
			return true;
		}
		else
		{
			return false;
		}
	#endif
}





bool cFile::IsFolder(const AString & a_Path)
{
	#ifdef _WIN32
		DWORD FileAttrib = GetFileAttributes(a_Path.c_str());
		return ((FileAttrib != INVALID_FILE_ATTRIBUTES) && ((FileAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0));
	#else
		struct stat st;
		return ((stat(a_Path.c_str(), &st) == 0) && S_ISDIR(st.st_mode));
	#endif
}





bool cFile::IsFile(const AString & a_Path)
{
	#ifdef _WIN32
		DWORD FileAttrib = GetFileAttributes(a_Path.c_str());
		return ((FileAttrib != INVALID_FILE_ATTRIBUTES) && ((FileAttrib & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE)) == 0));
	#else
		struct stat st;
		return ((stat(a_Path.c_str(), &st) == 0) && S_ISREG(st.st_mode));
	#endif
}





int cFile::GetSize(const AString & a_FileName)
{
	struct stat st;
	if (stat(a_FileName.c_str(), &st) == 0)
	{
		return (int)st.st_size;
	}
	return -1;
}





bool cFile::CreateFolder(const AString & a_FolderPath)
{
	#ifdef _WIN32
		return (CreateDirectory(a_FolderPath.c_str(), NULL) != 0);
	#else
		return (mkdir(a_FolderPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0);
	#endif
}





AStringVector cFile::GetFolderContents(const AString & a_Folder)
{
	AStringVector AllFiles;
	
	#ifdef _WIN32

	// If the folder name doesn't contain the terminating slash / backslash, add it:
	AString FileFilter = a_Folder;
	if (
		!FileFilter.empty() &&
		(FileFilter[FileFilter.length() - 1] != '\\') &&
		(FileFilter[FileFilter.length() - 1] != '/')
	)
	{
		FileFilter.push_back('\\');
	}
	
	// Find all files / folders:
	FileFilter.append("*.*");
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	if ((hFind = FindFirstFile(FileFilter.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			AllFiles.push_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	
	#else  // _WIN32

	DIR * dp;
	struct dirent *dirp;
	AString Folder = a_Folder;
	if (Folder.empty())
	{
		Folder = ".";
	}
	if ((dp = opendir(Folder.c_str())) == NULL)
	{
		LOGERROR("Error (%i) opening directory \"%s\"\n", errno, Folder.c_str());
	}
	else
	{
		while ((dirp = readdir(dp)) != NULL)
		{
			AllFiles.push_back(dirp->d_name);
		}
		closedir(dp);
	}
	
	#endif  // else _WIN32

	return AllFiles;
}





AString cFile::ReadWholeFile(const AString & a_FileName)
{
	cFile f;
	if (!f.Open(a_FileName, fmRead))
	{
		return "";
	}
	AString Contents;
	f.ReadRestOfFile(Contents);
	return Contents;
}





int cFile::Printf(const char * a_Fmt, ...)
{
	AString buf;
	va_list args;
	va_start(args, a_Fmt);
	AppendVPrintf(buf, a_Fmt, args);
	va_end(args);
	return Write(buf.c_str(), (int)buf.length());
}





void cFile::Flush(void)
{
	fflush(m_File);
}




