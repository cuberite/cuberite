
// StringUtils.cpp

// Implements the various string helper functions:

#include "Globals.h"





AString & AppendVPrintf(AString & str, const char *format, va_list args)
{
	ASSERT(format != NULL);
	
	char buffer[2048];
	size_t len;
	if ((len = _vsnprintf_s(buffer, ARRAYCOUNT(buffer), _TRUNCATE, format, args)) != -1)
	{
		str.append(buffer, len);
		return str;
	}
	len = _vscprintf(format, args);
	if (len == -1)
	{
		return str;
	}
	std::auto_ptr<char> tmp(new char[len + 1]);
	ASSERT(tmp.get() != NULL);  // Why not alloced? Is the length reasonable?
	if (tmp.get() == NULL)
	{
		throw std::bad_alloc();
	}
	if ((len = vsprintf_s(tmp.get(), len + 1, format, args)) != -1)
	{
		str.append(tmp.get(), len);
	}
	ASSERT(len != -1);
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
		if (cutAt > 0)
		{
			results.push_back(str.substr(0, cutAt));
		}
		Prev = cutAt + delim.length();
	}
	if (Prev < str.length())
	{
		results.push_back(str.substr(Prev));
	}
	return results;
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
	return stricmp(s1.c_str(), s2.c_str());
	#else 
	// Do it the hard way:
	AString s1Copy(s1);
	AString s2Copy(s2);
	return StrToUpper(s1Copy).compare(StrToUpper(s2Copy));
	#endif  // else _MSC_VER
}





