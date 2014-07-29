
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





AString & AppendVPrintf(AString & str, const char * format, va_list args)
{
	ASSERT(format != NULL);
	
	char buffer[2048];
	size_t len;
	#ifdef va_copy
	va_list argsCopy;
	va_copy(argsCopy, args);
	#else
	#define argsCopy args
	#endif
	#ifdef _MSC_VER
	// MS CRT provides secure printf that doesn't behave like in the C99 standard
	if ((len = _vsnprintf_s(buffer, ARRAYCOUNT(buffer), _TRUNCATE, format, argsCopy)) != -1)
	#else  // _MSC_VER
	if ((len = vsnprintf(buffer, ARRAYCOUNT(buffer), format, argsCopy)) < ARRAYCOUNT(buffer))
	#endif  // else _MSC_VER
	{
		// The result did fit into the static buffer
		#ifdef va_copy
		va_end(argsCopy);
		#endif
		str.append(buffer, len);
		return str;
	}
	#ifdef va_copy
	va_end(argsCopy);
	#endif
	
	// The result did not fit into the static buffer, use a dynamic buffer:
	#ifdef _MSC_VER
	// for MS CRT, we need to calculate the result length
	// MS doesn't have va_copy() and does nod need it at all
	len = _vscprintf(format, args);
	if (len == -1)
	{
		return str;
	}
	#endif  // _MSC_VER
	
	// Allocate a buffer and printf into it:
	#ifdef va_copy
	va_copy(argsCopy, args);
	#endif
	std::vector<char> Buffer(len + 1);
	#ifdef _MSC_VER
	vsprintf_s((char *)&(Buffer.front()), Buffer.size(), format, argsCopy);
	#else  // _MSC_VER
	vsnprintf((char *)&(Buffer.front()), Buffer.size(), format, argsCopy);
	#endif  // else _MSC_VER
	str.append(&(Buffer.front()), Buffer.size() - 1);
	#ifdef va_copy
	va_end(argsCopy);
	#endif
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





AString & AppendPrintf(AString &str, const char * format, ...)
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





AString & StrToLower(AString & s)
{
	AString::iterator i = s.begin();
	AString::iterator end = s.end();

	while (i != end)
	{
		*i = (char)tolower(*i);
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





size_t RateCompareString(const AString & s1, const AString & s2)
{
	size_t MatchedLetters = 0;
	size_t s1Length = s1.length();

	if (s1Length > s2.length())
	{
		// Definitely not a match
		return 0;
	}

	for (size_t i = 0; i < s1Length; i++)
	{
		char c1 = (char)toupper(s1[i]);
		char c2 = (char)toupper(s2[i]);
		if (c1 == c2)
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




// Converts a stream of BE shorts into UTF-8 string; returns a ref to a_UTF8
AString & RawBEToUTF8(const char * a_RawData, size_t a_NumShorts, AString & a_UTF8)
{
	a_UTF8.clear();
	a_UTF8.reserve(3 * a_NumShorts / 2);  // a quick guess of the resulting size
	for (size_t i = 0; i < a_NumShorts; i++)
	{
		int c = GetBEShort(&a_RawData[i * 2]);
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

////////////////////////////////////////////////////////////////////////////////
// Begin of Unicode, Inc.'s code / information
////////////////////////////////////////////////////////////////////////////////

/*
Notice from the original file:
* Copyright 2001-2004 Unicode, Inc.
*
* Disclaimer
*
* This source code is provided as is by Unicode, Inc. No claims are
* made as to fitness for any particular purpose. No warranties of any
* kind are expressed or implied. The recipient agrees to determine
* applicability of information provided. If this file has been
* purchased on magnetic or optical media from Unicode, Inc., the
* sole remedy for any claim will be exchange of defective media
* within 90 days of receipt.
*
* Limitations on Rights to Redistribute This Code
*
* Unicode, Inc. hereby grants the right to freely use the information
* supplied in this file in the creation of products supporting the
* Unicode Standard, and to make copies of this file in any form
* for internal or external distribution as long as this notice
* remains attached.
*/

#define UNI_MAX_BMP         0x0000FFFF
#define UNI_MAX_UTF16       0x0010FFFF
#define UNI_SUR_HIGH_START  0xD800
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

/*
---------------------------------------------------------------------
Note A.
The fall-through switches in UTF-8 reading code save a
temp variable, some decrements & conditionals.  The switches
are equivalent to the following loop:
{
	int tmpBytesToRead = extraBytesToRead + 1;
	do
	{
		ch += *source++;
		--tmpBytesToRead;
		if (tmpBytesToRead) ch <<= 6;
	} while (tmpBytesToRead > 0);
}
---------------------------------------------------------------------
*/

////////////////////////////////////////////////////////////////////////////////
// End of Unicode, Inc.'s code / information
////////////////////////////////////////////////////////////////////////////////






#define HEX(x) ((x) > 9 ? (x) + 'A' - 10 : (x) + '0')

/**
format binary data this way:
00001234: 31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66    1234567890abcdef
*/
AString & CreateHexDump(AString & a_Out, const void * a_Data, size_t a_Size, size_t a_BytesPerLine)
{
	ASSERT(a_BytesPerLine <= 120);  // Due to using a fixed size line buffer; increase line[]'s size to lift this max
	char line[512];
	char * p;
	char * q;
	
	a_Out.reserve(a_Size / a_BytesPerLine * (18 + 6 * a_BytesPerLine));
	for (size_t i = 0; i < a_Size; i += a_BytesPerLine)
	{
		size_t k = a_Size - i;
		if (k > a_BytesPerLine)
		{
			k = a_BytesPerLine;
		}
		#ifdef _MSC_VER
		// MSVC provides a "secure" version of sprintf()
		int Count = sprintf_s(line, sizeof(line), "%08x:", (unsigned)i);
		#else
		int Count = sprintf(line, "%08x:", (unsigned)i);
		#endif
		// Remove the terminating NULL / leftover garbage in line, after the sprintf-ed value
		memset(line + Count, 32, sizeof(line) - Count);
		p = line + 10;
		q = p + 2 + a_BytesPerLine * 3 + 1;
		for (size_t j = 0; j < k; j++)
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
	for (;;)
	{
		idx = res.find("\xc2\xa7", idx);
		if (idx == AString::npos)
		{
			return res;
		}
		res.erase(idx, 3);
	}
}





AString URLDecode(const AString & a_String)
{
	AString res;
	size_t len = a_String.length();
	res.reserve(len);
	for (size_t i = 0; i < len; i++)
	{
		char ch = a_String[i];
		if ((ch != '%') || (i > len - 3))
		{
			res.push_back(ch);
			continue;
		}
		// Decode the hex value:
		char hi = a_String[i + 1], lo = a_String[i + 2];
		if ((hi >= '0') && (hi <= '9'))
		{
			hi = hi - '0';
		}
		else if ((hi >= 'a') && (hi <= 'f'))
		{
			hi = hi - 'a' + 10;
		}
		else if ((hi >= 'A') && (hi <= 'F'))
		{
			hi = hi - 'F' + 10;
		}
		else
		{
			res.push_back(ch);
			continue;
		}
		if ((lo >= '0') && (lo <= '9'))
		{
			lo = lo - '0';
		}
		else if ((lo >= 'a') && (lo <= 'f'))
		{
			lo = lo - 'a' + 10;
		}
		else if ((lo >= 'A') && (lo <= 'F'))
		{
			lo = lo - 'A' + 10;
		}
		else
		{
			res.push_back(ch);
			continue;
		}
		res.push_back((hi << 4) | lo);
		i += 2;
	}  // for i - a_String[]
	return res;
}





AString ReplaceAllCharOccurrences(const AString & a_String, char a_From, char a_To)
{
	AString res(a_String);
	std::replace(res.begin(), res.end(), a_From, a_To);
	return res;
}





/// Converts one Hex character in a Base64 encoding into the data value
static inline int UnBase64(char c)
{
	if (c >='A' && c <= 'Z')
	{
		return c - 'A';
	}
	if (c >='a' && c <= 'z')
	{
		return c - 'a' + 26;
	}
	if (c >= '0' && c <= '9')
	{
		return c - '0' + 52;
	}
	if (c == '+')
	{
		return 62;
	}
	if (c == '/')
	{
		return 63;
	}
	if (c == '=')
	{
		return -1;
	}
	return -2;
}





AString Base64Decode(const AString & a_Base64String)
{
	AString res;
	size_t i, len = a_Base64String.size();
	int o, c;
	res.resize((len * 4) / 3 + 5, 0);  // Approximate the upper bound on the result length
	for (o = 0, i = 0; i < len; i++)
	{
		c = UnBase64(a_Base64String[i]);
		if (c >= 0)
		{
			switch (o & 7)
			{
				case 0: res[o >> 3] |= (c << 2); break;
				case 6: res[o >> 3] |= (c >> 4); res[(o >> 3) + 1] |= (c << 4); break;
				case 4: res[o >> 3] |= (c >> 2); res[(o >> 3) + 1] |= (c << 6); break;
				case 2: res[o >> 3] |= c; break;
			}
			o += 6;
		}
		if (c == -1)
		{
			// Error while decoding, invalid input. Return as much as we've decoded:
			res.resize(o >> 3);
			return res;
		}
	}
	res.resize(o >> 3);
	return res;
}





AString Base64Encode(const AString & a_Input)
{
	static const char BASE64[64] =
	{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
	};

	AString output;
	output.resize(((a_Input.size() + 2) / 3) * 4);

	size_t output_index = 0;
	size_t size_full24 = (a_Input.size() / 3) * 3;

	for (size_t i = 0; i < size_full24; i += 3)
	{
		output[output_index++] = BASE64[(unsigned char)a_Input[i] >> 2];
		output[output_index++] = BASE64[((unsigned char)a_Input[i] << 4 | (unsigned char)a_Input[i + 1] >> 4) & 63];
		output[output_index++] = BASE64[((unsigned char)a_Input[i + 1] << 2 | (unsigned char)a_Input[i + 2] >> 6) & 63];
		output[output_index++] = BASE64[(unsigned char)a_Input[i + 2] & 63];
	}

	if (size_full24 < a_Input.size())
	{
		output[output_index++] = BASE64[(unsigned char)a_Input[size_full24] >> 2];
		if (size_full24 + 1 == a_Input.size())
		{
			output[output_index++] = BASE64[((unsigned char)a_Input[size_full24] << 4) & 63];
			output[output_index++] = '=';
		}
		else
		{
			output[output_index++] = BASE64[((unsigned char)a_Input[size_full24] << 4 | (unsigned char)a_Input[size_full24 + 1] >> 4) & 63];
			output[output_index++] = BASE64[((unsigned char)a_Input[size_full24 + 1] << 2) & 63];
		}

		output[output_index++] = '=';
	}
	ASSERT(output_index == output.size());

	return output;
}





short GetBEShort(const char * a_Mem)
{
	const Byte * Bytes = (const Byte *)a_Mem;
	return (Bytes[0] << 8) | Bytes[1];
}





int GetBEInt(const char * a_Mem)
{
	const Byte * Bytes = (const Byte *)a_Mem;
	return (Bytes[0] << 24) | (Bytes[1] << 16) | (Bytes[2] << 8) | Bytes[3];
}





void SetBEInt(char * a_Mem, Int32 a_Value)
{
	a_Mem[0] = a_Value >> 24;
	a_Mem[1] = (a_Value >> 16) & 0xff;
	a_Mem[2] = (a_Value >> 8) & 0xff;
	a_Mem[3] = a_Value & 0xff;
}




