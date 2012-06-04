
// StringUtils.cpp

// Implements the various string helper functions:

#include "Globals.h"





AString & AppendVPrintf(AString & str, const char *format, va_list args)
{
	ASSERT(format != NULL);
	
	char buffer[2048];
	size_t len;
	#ifdef _MSC_VER
	// MS CRT provides secure printf that doesn't behave like in the C99 standard
	if ((len = _vsnprintf_s(buffer, ARRAYCOUNT(buffer), _TRUNCATE, format, args)) != -1)
	#else  // _MSC_VER
	if ((len = vsnprintf(buffer, ARRAYCOUNT(buffer), format, args)) < ARRAYCOUNT(buffer))
	#endif  // else _MSC_VER
	{
		// The result did fit into the static buffer
		str.append(buffer, len);
		return str;
	}
	
	// The result did not fit into the static buffer
	#ifdef _MSC_VER
	// for MS CRT, we need to calculate the result length
	len = _vscprintf(format, args);
	if (len == -1)
	{
		return str;
	}
	#endif  // _MSC_VER
	
	// Allocate a buffer and printf into it:
	str.resize(len + 1);
	// HACK: we're accessing AString's internal buffer in a way that is NOT guaranteed to always work. But it works on all STL implementations tested.
	// I can't think of any other way that is safe, doesn't allocate twice as much space as needed and doesn't use C++11 features like the move constructor
	#ifdef _MSC_VER
	vsprintf_s((char *)str.data(), len + 1, format, args);
	#else  // _MSC_VER
	vsnprintf((char *)str.data(), len + 1, format, args);
	#endif  // else _MSC_VER
	str.resize(len);
	return str;
}





AString & Printf(AString & str, const char *format, ...)
{
	str.clear();
	va_list args;
	va_start(args, format);
	std::string &retval = AppendVPrintf(str, format, args);
	va_end(args);
	return retval;
}





AString & AppendPrintf(AString &str, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	std::string &retval = AppendVPrintf(str, format, args);
	va_end(args);
	return retval;
}





AStringVector StringSplit(const AString & str, const AString & delim)
{
	AStringVector results;
	size_t cutAt = 0;
	size_t Prev = 0;
	while ((cutAt = str.find_first_of(delim, Prev)) != str.npos)
	{
		results.push_back(str.substr(Prev, cutAt - Prev));
		Prev = cutAt + delim.length();
	}
	if (Prev < str.length())
	{
		results.push_back(str.substr(Prev));
	}
	return results;
}




AString TrimString(const AString & str)
{
	size_t len = str.length();
	size_t start = 0;
	while (start < len)
	{
		if (str[start] > 32)
		{
			break;
		}
		++start;
	}
	if (start == len)
	{
		return "";
	}
	
	size_t end = len;
	while (end >= start)
	{
		if (str[end] > 32)
		{
			break;
		}
		--end;
	}
	
	return str.substr(start, end - start + 1);
}





AString & StrToUpper(AString & s)
{
	AString::iterator i = s.begin();
	AString::iterator end = s.end();

	while (i != end)
	{
		*i = (char)toupper(*i);
		++i;
	}
	return s;
}





int NoCaseCompare(const AString & s1, const AString & s2)
{
	#ifdef _MSC_VER
	// MSVC has stricmp that compares case-insensitive:
	return _stricmp(s1.c_str(), s2.c_str());
	#else 
	// Do it the hard way:
	AString s1Copy(s1);
	AString s2Copy(s2);
	return StrToUpper(s1Copy).compare(StrToUpper(s2Copy));
	#endif  // else _MSC_VER
}





void ReplaceString(AString & iHayStack, const AString & iNeedle, const AString & iReplaceWith)
{
	size_t pos1 = iHayStack.find(iNeedle);
	while (pos1 != AString::npos)
	{
		iHayStack.replace( pos1, iNeedle.size(), iReplaceWith);
		pos1 = iHayStack.find(iNeedle, pos1);
	}
}




AStringList GetDirectoryContents(const char * a_Directory)
{
	AStringList AllFiles;
	
	#ifdef _WIN32
	
	AString FileFilter = AString(a_Directory) + "*.*";
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
	if (*a_Directory == 0)
	{
		a_Directory = ".";
	}
	if ((dp = opendir(a_Directory)) == NULL) 
	{
		LOGERROR("Error (%i) opening directory \"%s\"\n", errno, a_Directory );
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




