
// StringUtils.cpp

// Implements the various string helper functions:

#include "Globals.h"

#if defined(ANDROID_NDK)
#include <ctype.h>
#endif

#ifdef _MSC_VER
	// Under MSVC, link to WinSock2 (needed by RawBEToUTF8's byteswapping)
	#pragma comment(lib, "ws2_32.lib")
#endif





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





AString & Printf(AString & str, const char * format, ...)
{
	str.clear();
	va_list args;
	va_start(args, format);
	std::string &retval = AppendVPrintf(str, format, args);
	va_end(args);
	return retval;
}





AString Printf(const char * format, ...)
{
	AString res;
	va_list args;
	va_start(args, format);
	AppendVPrintf(res, format, args);
	va_end(args);
	return res;
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
		Prev = cutAt + 1;
	}
	if (Prev < str.length())
	{
		results.push_back(str.substr(Prev));
	}
	return results;
}





AStringVector StringSplitAndTrim(const AString & str, const AString & delim)
{
	AStringVector results;
	size_t cutAt = 0;
	size_t Prev = 0;
	while ((cutAt = str.find_first_of(delim, Prev)) != str.npos)
	{
		results.push_back(TrimString(str.substr(Prev, cutAt - Prev)));
		Prev = cutAt + 1;
	}
	if (Prev < str.length())
	{
		results.push_back(TrimString(str.substr(Prev)));
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





unsigned int RateCompareString(const AString & s1, const AString & s2 )
{
	unsigned int MatchedLetters = 0;
	unsigned int s1Length = s1.length();

	if( s1Length > s2.length() ) return 0; // Definitely not a match

	for (unsigned int i = 0; i < s1Length; i++)
	{
		char c1 = (char)toupper( s1[i] );
		char c2 = (char)toupper( s2[i] );
		if( c1 == c2 )
		{
			++MatchedLetters;
		}
		else
		{
			break;
		}
	}
	return MatchedLetters;
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





// Converts a stream of BE shorts into UTF-8 string; returns a ref to a_UTF8
AString & RawBEToUTF8(short * a_RawData, int a_NumShorts, AString & a_UTF8)
{
	a_UTF8.clear();
	a_UTF8.reserve(3 * a_NumShorts / 2);  // a quick guess of the resulting size
	for (int i = 0; i < a_NumShorts; i++)
	{
		int c = ntohs(*(a_RawData + i));
		if (c < 0x80)
		{
			a_UTF8.push_back((char)c);
		}
		else if (c < 0x800)
		{
			a_UTF8.push_back((char)(192 + c / 64));
			a_UTF8.push_back((char)(128 + c % 64));
		}
		else if (c - 0xd800u < 0x800)
		{
			// Error, silently drop
		}
		else if (c < 0x10000)
		{
			a_UTF8.push_back((char)(224 + c / 4096));
			a_UTF8.push_back((char)(128 + c / 64 % 64));
			a_UTF8.push_back((char)(128 + c % 64));
		}
		else if (c < 0x110000)
		{
			a_UTF8.push_back((char)(240 + c / 262144));
			a_UTF8.push_back((char)(128 + c / 4096 % 64));
			a_UTF8.push_back((char)(128 + c / 64 % 64));
			a_UTF8.push_back((char)(128 + c % 64));
		}
		else
		{
			// Error, silently drop
		}
	}
	return a_UTF8;
}




// UTF-8 conversion code adapted from:
//  http://stackoverflow.com/questions/2867123/convert-utf-16-to-utf-8-under-windows-and-linux-in-c

#define UNI_MAX_BMP         0x0000FFFF
#define UNI_MAX_UTF16       0x0010FFFF
#define UNI_MAX_UTF32       0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 0x0010FFFF
#define UNI_SUR_HIGH_START  0xD800
#define UNI_SUR_HIGH_END    0xDBFF
#define UNI_SUR_LOW_START   0xDC00
#define UNI_SUR_LOW_END     0xDFFF





static const char trailingBytesForUTF8[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};





static const unsigned int offsetsFromUTF8[6] =
{
	0x00000000UL, 0x00003080UL, 0x000E2080UL, 
	0x03C82080UL, 0xFA082080UL, 0x82082080UL
};





static bool isLegalUTF8(const unsigned char * source, int length)
{
	unsigned char a;
	const unsigned char * srcptr = source + length;
	switch (length)
	{
		default: return false;
		// Everything else falls through when "true"...
		case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 2:
		{
			if ((a = (*--srcptr)) > 0xBF) return false;
			switch (*source)
			{
				// no fall-through in this inner switch
				case 0xE0: if (a < 0xA0) return false; break;
				case 0xED: if (a > 0x9F) return false; break;
				case 0xF0: if (a < 0x90) return false; break;
				case 0xF4: if (a > 0x8F) return false; break;
				default:   if (a < 0x80) return false;
			}
		}
		case 1: if (*source >= 0x80 && *source < 0xC2) return false;
	}
	if (*source > 0xF4) return false;
	return true;
}





AString & UTF8ToRawBEUTF16(const char * a_UTF8, size_t a_UTF8Length, AString & a_UTF16)
{
	a_UTF16.clear();
	a_UTF16.reserve(a_UTF8Length * 3);

	const unsigned char * source    = (const unsigned char*)a_UTF8;
	const unsigned char * sourceEnd = source + a_UTF8Length;
	const int halfShift  = 10;  // used for shifting by 10 bits
	const unsigned int halfBase = 0x0010000UL;
	const unsigned int halfMask = 0x3FFUL;

	while (source < sourceEnd)
	{
		unsigned int ch = 0;
		unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
		if (source + extraBytesToRead >= sourceEnd)
		{
			return a_UTF16;
		}
		// Do this check whether lenient or strict
		if (!isLegalUTF8(source, extraBytesToRead + 1))
		{
			return a_UTF16;
			break;
		}
		
		// The cases all fall through. See "Note A" below.
		switch (extraBytesToRead)
		{
			case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
			case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
			case 3: ch += *source++; ch <<= 6;
			case 2: ch += *source++; ch <<= 6;
			case 1: ch += *source++; ch <<= 6;
			case 0: ch += *source++;
		}
		ch -= offsetsFromUTF8[extraBytesToRead];

		if (ch <= UNI_MAX_BMP)
		{
			// Target is a character <= 0xFFFF
			if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
			{
				// UTF-16 surrogate values are illegal in UTF-32
				ch = ' ';
			}
			unsigned short v = htons((unsigned short)ch);
			a_UTF16.append((const char *)&v, 2);
		}
		else if (ch > UNI_MAX_UTF16)
		{
			// Invalid value, replace with a space
			unsigned short v = htons(' ');
			a_UTF16.append((const char *)&v, 2);
		}
		else
		{
			// target is a character in range 0xFFFF - 0x10FFFF.
			ch -= halfBase;
			unsigned short v1 = htons((ch >> halfShift) + UNI_SUR_HIGH_START);
			unsigned short v2 = htons((ch & halfMask) + UNI_SUR_LOW_START);
			a_UTF16.append((const char *)&v1, 2);
			a_UTF16.append((const char *)&v2, 2);
		}
	}
	return a_UTF16;
}

/* ---------------------------------------------------------------------

    Note A.
    The fall-through switches in UTF-8 reading code save a
    temp variable, some decrements & conditionals.  The switches
    are equivalent to the following loop:
    {
        int tmpBytesToRead = extraBytesToRead+1;
        do {
        ch += *source++;
        --tmpBytesToRead;
        if (tmpBytesToRead) ch <<= 6;
        } while (tmpBytesToRead > 0);
    }

   --------------------------------------------------------------------- */
   




/*
format binary data this way:
00001234: 31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66    1234567890abcdef
*/
#define HEX(x) ((x) > 9 ? (x) + 'A' - 10 : (x) + '0')

AString & CreateHexDump(AString & a_Out, const void * a_Data, int a_Size, int a_LineLength)
{
	ASSERT(a_LineLength <= 120);  // Due to using a fixed size line buffer; increase line[]'s size to lift this max
	char line[512];
	char * p;
	char * q;
	
	a_Out.reserve(a_Size / a_LineLength * (18 + 6 * a_LineLength));
	for (int i = 0; i < a_Size; i += a_LineLength)
	{
		int k = a_Size - i;
		if (k > a_LineLength)
		{
			k = a_LineLength;
		}
		#ifdef _MSC_VER
		// MSVC provides a "secure" version of sprintf()
		int Count = sprintf_s(line, sizeof(line), "%08x:", i);
		#else
		int Count = sprintf(line, "%08x:", i);
		#endif
		// Remove the terminating NULL / leftover garbage in line, after the sprintf-ed value
		memset(line + Count, 32, sizeof(line) - Count);
		p = line + 10;
		q = p + 2 + a_LineLength * 3 + 1;
		for (int j = 0; j < k; j++)
		{
			unsigned char c = ((unsigned char *)a_Data)[i + j];
			p[0] = HEX(c >> 4);
			p[1] = HEX(c & 0xf);
			p[2] = ' ';
			if (c >= ' ')
			{
				q[0] = (char)c;
			}
			else
			{
				q[0] = '.';
			}
			p += 3;
			q ++;
		}  // for j
		q[0] = '\n';
		q[1] = 0;
		a_Out.append(line);
	}  // for i
	return a_Out;
}





AString EscapeString(const AString & a_Message)
{
	AString EscapedMsg;
	size_t len = a_Message.size();
	size_t last = 0;
	EscapedMsg.reserve(len);
	for (size_t i = 0; i < len; i++)
	{
		char ch = a_Message[i];
		switch (ch)
		{
			case '\'':
			case '\"':
			case '\\':
			{
				if (i > last)
				{
					EscapedMsg.append(a_Message, last, i - last);
				}
				EscapedMsg.push_back('\\');
				EscapedMsg.push_back(ch);
				last = i + 1;
				break;
			}
		}  // switch (ch)
	}  // for i - a_Message[]
	if (len > last)
	{
		EscapedMsg.append(a_Message, last, len - last);
	}
	return EscapedMsg;
}





AString StripColorCodes(const AString & a_Message)
{
	AString res(a_Message);
	size_t idx = 0;
	while (true)
	{
		idx = res.find("\xc2\xa7", idx);
		if (idx == AString::npos)
		{
			return res;
		}
		res.erase(idx, 3);
	}
}




