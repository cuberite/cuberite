
// StringUtils.cpp

// Implements the various string helper functions:

#include "Globals.h"

#include "fmt/printf.h"

#ifdef _MSC_VER
	// Under MSVC, link to WinSock2 (needed by RawBEToUTF8's byteswapping)
	#pragma comment(lib, "ws2_32.lib")
#endif





/** Returns the value of the single hex digit.
Returns 0xff on failure. */
static unsigned char HexToDec(char a_HexChar)
{
	switch (a_HexChar)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
	}
	return 0xff;
}





AString & Printf(AString & a_str, const char * a_format, fmt::ArgList a_args)
{
	ASSERT(format != nullptr);
	a_str = fmt::sprintf(a_format, a_args);
	return a_str;
}





AString Printf(const char * a_format, fmt::ArgList a_args)
{
	ASSERT(format != nullptr);
	return fmt::sprintf(a_format, a_args);
}





AStringVector StringSplit(const AString & a_str, const AString & a_delim)
{
	AStringVector results;
	size_t cutAt = 0;
	size_t Prev = 0;
	while ((cutAt = a_str.find_first_of(a_delim, Prev)) != a_str.npos)
	{
		results.push_back(a_str.substr(Prev, cutAt - Prev));
		Prev = cutAt + 1;
	}
	if (Prev < a_str.length())
	{
		results.push_back(a_str.substr(Prev));
	}
	return results;
}




AStringVector StringSplitWithQuotes(const AString & a_str, const AString & a_delim)
{
	AStringVector results;

	size_t cutAt = 0;
	size_t Prev = 0;
	size_t cutAtQuote = 0;

	while ((cutAt = a_str.find_first_of(a_delim, Prev)) != a_str.npos)
	{
		if (cutAt == Prev)
		{
			// Empty string due to multiple whitespace / whitespace at the beginning of the input
			// Just skip it
			Prev = Prev + 1;
			continue;
		}
		AString current = a_str.substr(Prev, cutAt - Prev);
		if ((current.front() == '"') || (current.front() == '\''))
		{
			Prev += 1;
			cutAtQuote = a_str.find_first_of(current.front(), Prev);
			if (cutAtQuote != a_str.npos)
			{
				current = a_str.substr(Prev, cutAtQuote - Prev);
				cutAt = cutAtQuote + 1;
			}
		}

		results.push_back(std::move(current));
		Prev = cutAt + 1;
	}

	if (Prev < a_str.length())
	{
		AString current = a_str.substr(Prev);

		// If the remant is wrapped in matching quotes, remove them:
		if (
			(current.length() >= 2) &&
			((current.front() == '"') || (current.front() == '\'')) &&
			(current.front() == current.back())
		)
		{
			current = current.substr(1, current.length() - 2);
		}

		results.push_back(current);
	}

	return results;
}





AString StringJoin(const AStringVector & a_Strings, const AString & a_Delimeter)
{
	if (a_Strings.empty())
	{
		return {};
	}

	// Do a dry run to gather the size
	const auto DelimSize = a_Delimeter.size();
	size_t ResultSize = a_Strings[0].size();
	std::for_each(a_Strings.begin() + 1, a_Strings.end(),
		[&](const AString & a_String)
		{
			ResultSize += DelimSize;
			ResultSize += a_String.size();
		}
	);

	// Now do the actual join
	AString Result;
	Result.reserve(ResultSize);
	Result.append(a_Strings[0]);
	std::for_each(a_Strings.begin() + 1, a_Strings.end(),
		[&](const AString & a_String)
		{
			Result += a_Delimeter;
			Result += a_String;
		}
	);
	return Result;
}




AStringVector StringSplitAndTrim(const AString & a_str, const AString & a_delim)
{
	AStringVector results;
	size_t cutAt = 0;
	size_t Prev = 0;
	while ((cutAt = a_str.find_first_of(a_delim, Prev)) != a_str.npos)
	{
		results.push_back(TrimString(a_str.substr(Prev, cutAt - Prev)));
		Prev = cutAt + 1;
	}
	if (Prev < a_str.length())
	{
		results.push_back(TrimString(a_str.substr(Prev)));
	}
	return results;
}




AString TrimString(const AString & a_str)
{
	size_t len = a_str.length();
	size_t start = 0;
	while (start < len)
	{
		if (static_cast<unsigned char>(a_str[start]) > 32)
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
		if (static_cast<unsigned char>(a_str[end]) > 32)
		{
			break;
		}
		--end;
	}

	return a_str.substr(start, end - start + 1);
}





AString & InPlaceLowercase(AString & a_s)
{
	std::transform(a_s.begin(), a_s.end(), a_s.begin(), ::tolower);
	return a_s;
}





AString & InPlaceUppercase(AString & a_s)
{
	std::transform(a_s.begin(), a_s.end(), a_s.begin(), ::toupper);
	return a_s;
}





AString StrToLower(const AString & a_s)
{
	AString res;
	res.resize(a_s.size());
	std::transform(a_s.begin(), a_s.end(), res.begin(), ::tolower);
	return res;
}





AString StrToUpper(const AString & a_s)
{
	AString res;
	res.resize(a_s.size());
	std::transform(a_s.begin(), a_s.end(), res.begin(), ::toupper);
	return res;
}





int NoCaseCompare(const AString & a_s1, const AString & a_s2)
{
	#ifdef _MSC_VER
		return _stricmp(s1.c_str(), s2.c_str());
	#else
		return strcasecmp(a_s1.c_str(), a_s2.c_str());
	#endif  // else _MSC_VER
}





size_t RateCompareString(const AString & a_s1, const AString & a_s2)
{
	size_t MatchedLetters = 0;
	size_t s1Length = a_s1.length();

	if (s1Length > a_s2.length())
	{
		// Definitely not a match
		return 0;
	}

	for (size_t i = 0; i < s1Length; i++)
	{
		char c1 = static_cast<char>(toupper(a_s1[i]));
		char c2 = static_cast<char>(toupper(a_s2[i]));
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





void ReplaceString(AString & a_iHayStack, const AString & a_iNeedle, const AString & a_iReplaceWith)
{
	// find always returns the current position for an empty needle; prevent endless loop
	if (a_iNeedle.empty())
	{
		return;
	}

	size_t pos1 = a_iHayStack.find(a_iNeedle);
	while (pos1 != AString::npos)
	{
		a_iHayStack.replace( pos1, a_iNeedle.size(), a_iReplaceWith);
		pos1 = a_iHayStack.find(a_iNeedle, pos1 + a_iReplaceWith.size());
	}
}




AString & RawBEToUTF8(const char * a_RawData, size_t a_NumShorts, AString & a_UTF8)
{
	a_UTF8.clear();
	a_UTF8.reserve(3 * a_NumShorts / 2);  // a quick guess of the resulting size
	for (size_t i = 0; i < a_NumShorts; i++)
	{
		a_UTF8.append(UnicodeCharToUtf8(GetBEUShort(&a_RawData[i * 2])));
	}
	return a_UTF8;
}




AString UnicodeCharToUtf8(unsigned a_UnicodeChar)
{
	if (a_UnicodeChar < 0x80)
	{
		return AString{static_cast<char>(a_UnicodeChar)};
	}
	else if (a_UnicodeChar < 0x800)
	{
		return AString
		{
			static_cast<char>(192 + a_UnicodeChar / 64),
			static_cast<char>(128 + a_UnicodeChar % 64),
		};
	}
	else if (a_UnicodeChar - 0xd800 < 0x800)
	{
		// Error
		return AString();
	}
	else if (a_UnicodeChar < 0x10000)
	{
		return AString
		{
			static_cast<char>(224 + a_UnicodeChar / 4096),
			static_cast<char>(128 + (a_UnicodeChar / 64) % 64),
			static_cast<char>(128 + a_UnicodeChar % 64)
		};
	}
	else if (a_UnicodeChar < 0x110000)
	{
		return AString
		{
			static_cast<char>(240 + a_UnicodeChar / 262144),
			static_cast<char>(128 + (a_UnicodeChar / 4096) % 64),
			static_cast<char>(128 + (a_UnicodeChar / 64) % 64),
			static_cast<char>(128 + a_UnicodeChar % 64),
		};
	}
	else
	{
		// Error
		return AString();
	}
}





// UTF-8 conversion code adapted from:
//  https://stackoverflow.com/questions/2867123/convert-utf-16-to-utf-8-under-windows-and-linux-in-c

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





static const Byte g_trailingBytesForUTF8[256] =
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





static const unsigned int g_offsetsFromUTF8[6] =
{
	0x00000000UL, 0x00003080UL, 0x000E2080UL,
	0x03C82080UL, 0xFA082080UL, 0x82082080UL
};





static bool isLegalUTF8(const unsigned char * a_source, int a_length)
{
	unsigned char a;
	const unsigned char * srcptr = a_source + a_length;
	switch (a_length)
	{
		default: return false;
		// Everything else falls through when "true"...
		case 4: if (((a = (*--srcptr)) < 0x80) || (a > 0xbf)) return false;
		case 3: if (((a = (*--srcptr)) < 0x80) || (a > 0xbf)) return false;
		case 2:
		{
			if ((a = (*--srcptr)) > 0xbf)
			{
				return false;
			}
			switch (*a_source)
			{
				// no fall-through in this inner switch
				case 0xe0: if (a < 0xa0) return false; break;
				case 0xed: if (a > 0x9f) return false; break;
				case 0xf0: if (a < 0x90) return false; break;
				case 0xf4: if (a > 0x8f) return false; break;
				default:   if (a < 0x80) return false;
			}
		}
		case 1: if ((*a_source >= 0x80) && (*a_source < 0xc2)) return false;
	}
	if (*a_source > 0xf4)
	{
		return false;
	}
	return true;
}





std::u16string UTF8ToRawBEUTF16(const AString & a_UTF8)
{
	std::u16string UTF16;
	UTF16.reserve(a_UTF8.size() * 2);

	const unsigned char * source    = reinterpret_cast<const unsigned char *>(a_UTF8.data());
	const unsigned char * sourceEnd = source + a_UTF8.size();
	const int halfShift  = 10;  // used for shifting by 10 bits
	const unsigned int halfBase = 0x0010000UL;
	const unsigned int halfMask = 0x3ffUL;

	while (source < sourceEnd)
	{
		unsigned int ch = 0;
		unsigned short extraBytesToRead = g_trailingBytesForUTF8[*source];
		if (source + extraBytesToRead >= sourceEnd)
		{
			return UTF16;
		}
		// Do this check whether lenient or strict
		if (!isLegalUTF8(source, extraBytesToRead + 1))
		{
			return UTF16;
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
		ch -= g_offsetsFromUTF8[extraBytesToRead];

		if (ch <= UNI_MAX_BMP)
		{
			// Target is a character <= 0xFFFF
			if ((ch >= UNI_SUR_HIGH_START) && (ch <= UNI_SUR_LOW_END))
			{
				// UTF-16 surrogate values are illegal in UTF-32
				ch = ' ';
			}
			unsigned short v = htons(static_cast<unsigned short>(ch));
			UTF16.push_back(static_cast<char16_t>(v));
		}
		else if (ch > UNI_MAX_UTF16)
		{
			// Invalid value, replace with a space
			unsigned short v = htons(' ');
			UTF16.push_back(static_cast<char16_t>(v));
		}
		else
		{
			// target is a character in range 0xFFFF - 0x10FFFF.
			ch -= halfBase;
			auto v1 = htons(static_cast<uint16_t>((ch >> halfShift) + UNI_SUR_HIGH_START));
			auto v2 = htons(static_cast<uint16_t>((ch & halfMask) + UNI_SUR_LOW_START));
			UTF16.push_back(static_cast<char16_t>(v1));
			UTF16.push_back(static_cast<char16_t>(v2));
		}
	}
	return UTF16;
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
		if (tmpBytesToRead)
		{
			ch <<= 6;
		}
	} while (tmpBytesToRead > 0);
}
---------------------------------------------------------------------
*/

////////////////////////////////////////////////////////////////////////////////
// End of Unicode, Inc.'s code / information
////////////////////////////////////////////////////////////////////////////////






#define HEX(x) static_cast<char>((x) > 9 ? (x) + 'A' - 10 : (x) + '0')

/**
format binary data this way:
00001234: 31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66    1234567890abcdef
*/
AString & CreateHexDump(AString & a_Out, const void * a_Data, size_t a_Size, size_t a_BytesPerLine)
{
	fmt::MemoryWriter Output;
	/* If formatting the data from the comment above:
		Hex holds:   "31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66 "
		Chars holds: "1234567890abcdef" */
	fmt::MemoryWriter Hex, Chars;

	if (a_Size > 0)
	{
		// Same as std::ceil(static_cast<float>(a_Size) / a_BytesPerLine);
		const size_t NumLines = a_Size / a_BytesPerLine + (a_Size % a_BytesPerLine != 0);
		const size_t CharsPerLine = 14 + 4 * a_BytesPerLine;
		Output.buffer().reserve(NumLines * CharsPerLine);
	}

	for (size_t i = 0; i < a_Size; i += a_BytesPerLine)
	{
		size_t k = std::min(a_Size - i, a_BytesPerLine);
		for (size_t j = 0; j < k; j++)
		{
			Byte c = (static_cast<const Byte *>(a_Data))[i + j];
			Hex << HEX(c >> 4) << HEX(c & 0xf) << ' ';
			Chars << ((c >= ' ') ? static_cast<char>(c) : '.');
		}  // for j

		// Write Hex with a dynamic fixed width
		Output.write("{0:08x}: {1:{2}}   {3}\n", i, Hex.c_str(), a_BytesPerLine * 3, Chars.c_str());
		Hex.clear();
		Chars.clear();
	}  // for i
	a_Out.append(Output.data(), Output.size());
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





std::pair<bool, AString> URLDecode(const AString & a_Text)
{
	AString res;
	auto len = a_Text.size();
	res.reserve(len);
	for (size_t i = 0; i < len; i++)
	{
		if (a_Text[i] == '+')
		{
			res.push_back(' ');
			continue;
		}
		if (a_Text[i] != '%')
		{
			res.push_back(a_Text[i]);
			continue;
		}
		if (i + 1 >= len)
		{
			// String too short for an encoded value
			return std::make_pair(false, AString());
		}
		if ((a_Text[i + 1] == 'u') || (a_Text[i + 1] == 'U'))
		{
			// Unicode char "%u0xxxx"
			if (i + 6 >= len)
			{
				return std::make_pair(false, AString());
			}
			if (a_Text[i + 2] != '0')
			{
				return std::make_pair(false, AString());
			}
			unsigned v1 = HexToDec(a_Text[i + 3]);
			unsigned v2 = HexToDec(a_Text[i + 4]);
			unsigned v3 = HexToDec(a_Text[i + 5]);
			unsigned v4 = HexToDec(a_Text[i + 6]);
			if ((v1 == 0xff) || (v2 == 0xff) || (v4 == 0xff) || (v3 == 0xff))
			{
				// Invalid hex numbers
				return std::make_pair(false, AString());
			}
			res.append(UnicodeCharToUtf8((v1 << 12) | (v2 << 8) | (v3 << 4) | v4));
			i = i + 6;
		}
		else
		{
			// Regular char "%xx":
			if (i + 2 >= len)
			{
				return std::make_pair(false, AString());
			}
			auto v1 = HexToDec(a_Text[i + 1]);
			auto v2 = HexToDec(a_Text[i + 2]);
			if ((v1 == 0xff) || (v2 == 0xff))
			{
				// Invalid hex numbers
				return std::make_pair(false, AString());
			}
			res.push_back(static_cast<char>((v1 << 4) | v2));
			i = i + 2;
		}
	}  // for i - a_Text[i]
	return std::make_pair(true, res);
}





AString URLEncode(const AString & a_Text)
{
	AString res;
	auto len = a_Text.size();
	res.reserve(len);
	static const char HEX[] = "0123456789abcdef";
	for (size_t i = 0; i < len; ++i)
	{
		if (isalnum(a_Text[i]))
		{
			res.push_back(a_Text[i]);
		}
		else if (a_Text[i] == ' ')
		{
			res.push_back('+');
		}
		else
		{
			res.push_back('%');
			res.push_back(HEX[static_cast<unsigned char>(a_Text[i]) >> 4]);
			res.push_back(HEX[static_cast<unsigned char>(a_Text[i]) & 0x0f]);
		}
	}
	return res;
}





AString ReplaceAllCharOccurrences(const AString & a_String, char a_From, char a_To)
{
	AString res(a_String);
	std::replace(res.begin(), res.end(), a_From, a_To);
	return res;
}





/** Converts one Hex character in a Base64 encoding into the data value */
static inline int UnBase64(char a_c)
{
	if ((a_c >='A') && (a_c <= 'Z'))
	{
		return a_c - 'A';
	}
	if ((a_c >='a') && (a_c <= 'z'))
	{
		return a_c - 'a' + 26;
	}
	if ((a_c >= '0') && (a_c <= '9'))
	{
		return a_c - '0' + 52;
	}
	if (a_c == '+')
	{
		return 62;
	}
	if (a_c == '/')
	{
		return 63;
	}
	if (a_c == '=')
	{
		return -1;
	}
	return -2;
}





AString Base64Decode(const AString & a_Base64String)
{
	AString res;
	size_t i, len = a_Base64String.size();
	size_t o;
	int c;
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
		output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[i]) >> 2];
		output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[i]) << 4 | static_cast<unsigned char>(a_Input[i + 1]) >> 4) & 63];
		output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[i + 1]) << 2 | static_cast<unsigned char>(a_Input[i + 2]) >> 6) & 63];
		output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[i + 2]) & 63];
	}

	if (size_full24 < a_Input.size())
	{
		output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[size_full24]) >> 2];
		if (size_full24 + 1 == a_Input.size())
		{
			output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24]) << 4) & 63];
			output[output_index++] = '=';
		}
		else
		{
			output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24]) << 4 | static_cast<unsigned char>(a_Input[size_full24 + 1]) >> 4) & 63];
			output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24 + 1]) << 2) & 63];
		}

		output[output_index++] = '=';
	}
	ASSERT(output_index == output.size());

	return output;
}





short GetBEShort(const char * a_Mem)
{
	const Byte * Bytes = reinterpret_cast<const Byte *>(a_Mem);
	return static_cast<short>((Bytes[0] << 8) | Bytes[1]);
}





unsigned short GetBEUShort(const char * a_Mem)
{
	const Byte * Bytes = reinterpret_cast<const Byte *>(a_Mem);
	return static_cast<unsigned short>((Bytes[0] << 8) | Bytes[1]);
}





int GetBEInt(const char * a_Mem)
{
	const Byte * Bytes = reinterpret_cast<const Byte *>(a_Mem);
	return (Bytes[0] << 24) | (Bytes[1] << 16) | (Bytes[2] << 8) | Bytes[3];
}





void SetBEInt(char * a_Mem, Int32 a_Value)
{
	a_Mem[0] = a_Value >> 24;
	a_Mem[1] = static_cast<char>((a_Value >> 16) & 0xff);
	a_Mem[2] = static_cast<char>((a_Value >> 8) & 0xff);
	a_Mem[3] = static_cast<char>(a_Value & 0xff);
}





bool SplitZeroTerminatedStrings(const AString & a_Strings, AStringVector & a_Output)
{
	a_Output.clear();
	size_t size = a_Strings.size();
	size_t start = 0;
	bool res = false;
	for (size_t i = 0; i < size; i++)
	{
		if (a_Strings[i] == 0)
		{
			a_Output.push_back(a_Strings.substr(start, i - start));
			start = i + 1;
			res = true;
		}
	}
	if (start < size)
	{
		a_Output.push_back(a_Strings.substr(start, size - start));
		res = true;
	}

	return res;
}





AStringVector MergeStringVectors(const AStringVector & a_Strings1, const AStringVector & a_Strings2)
{
	// Initialize the resulting vector by the first vector:
	AStringVector res = a_Strings1;

	// Add each item from strings2 that is not already present:
	for (auto item : a_Strings2)
	{
		if (std::find(res.begin(), res.end(), item) == res.end())
		{
			res.push_back(item);
		}
	}  // for item - a_Strings2[]

	return res;
}





AString StringsConcat(const AStringVector & a_Strings, char a_Separator)
{
	// If the vector is empty, return an empty string:
	if (a_Strings.empty())
	{
		return "";
	}

	// Concatenate the strings in the vector:
	AString res;
	res.append(a_Strings[0]);
	for (auto itr = a_Strings.cbegin() + 1, end = a_Strings.cend(); itr != end; ++itr)
	{
		res.push_back(a_Separator);
		res.append(*itr);
	}
	return res;
}

bool StringToFloat(const AString & a_String, float & a_Num)
{
	char *err;
	a_Num = strtof(a_String.c_str(), &err);
	if (*err != 0)
	{
		return false;
	}
	return true;
}

