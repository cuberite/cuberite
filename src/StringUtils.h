
// StringUtils.h

// Interfaces to various string helper functions




#pragma once

#include <string>




typedef std::string AString;
typedef std::vector<AString> AStringVector;
typedef std::list<AString>   AStringList;





/** Add the formated string to the existing data in the string */
extern AString & AppendVPrintf(AString & str, const char * format, va_list args) FORMATSTRING(2, 0);

/// Output the formatted text into the string
extern AString & Printf       (AString & str, const char * format, ...) FORMATSTRING(2, 3);

/// Output the formatted text into string, return string by value
extern AString Printf(const char * format, ...) FORMATSTRING(1, 2);

/// Add the formatted string to the existing data in the string
extern AString & AppendPrintf (AString & str, const char * format, ...) FORMATSTRING(2, 3);

/// Split the string at any of the listed delimiters, return as a stringvector
extern AStringVector StringSplit(const AString & str, const AString & delim);

/// Split the string at any of the listed delimiters and trim each value, return as a stringvector
extern AStringVector StringSplitAndTrim(const AString & str, const AString & delim);

/// Trime whitespace at both ends of the string
extern AString TrimString(const AString & str);  // tolua_export

/// In-place string conversion to uppercase; returns the same string
extern AString & StrToUpper(AString & s);

/// In-place string conversion to lowercase; returns the same string
extern AString & StrToLower(AString & s);

/// Case-insensitive string comparison; returns 0 if the strings are the same
extern int NoCaseCompare(const AString & s1, const AString & s2);  // tolua_export

/// Case-insensitive string comparison that returns a rating of equal-ness between [0 - s1.length()]
extern size_t RateCompareString(const AString & s1, const AString & s2);

/// Replaces *each* occurence of iNeedle in iHayStack with iReplaceWith
extern void ReplaceString(AString & iHayStack, const AString & iNeedle, const AString & iReplaceWith);  // tolua_export

/// Converts a stream of BE shorts into UTF-8 string; returns a ref to a_UTF8
extern AString & RawBEToUTF8(const char * a_RawData, size_t a_NumShorts, AString & a_UTF8);

/// Converts a UTF-8 string into a UTF-16 BE string, packing that back into AString; return a ref to a_UTF16
extern AString & UTF8ToRawBEUTF16(const char * a_UTF8, size_t a_UTF8Length, AString & a_UTF16);

/// Creates a nicely formatted HEX dump of the given memory block. Max a_BytesPerLine is 120
extern AString & CreateHexDump(AString & a_Out, const void * a_Data, size_t a_Size, size_t a_BytesPerLine);

/// Returns a copy of a_Message with all quotes and backslashes escaped by a backslash
extern AString EscapeString(const AString & a_Message);  // tolua_export

/// Removes all control codes used by MC for colors and styles
extern AString StripColorCodes(const AString & a_Message);  // tolua_export

/// URL-Decodes the given string, replacing all "%HH" into the correct characters. Invalid % sequences are left intact
extern AString URLDecode(const AString & a_String);  // Cannot export to Lua automatically - would generated an extra return value

/// Replaces all occurrences of char a_From inside a_String with char a_To.
extern AString ReplaceAllCharOccurrences(const AString & a_String, char a_From, char a_To);  // Needn't export to Lua, since Lua doesn't have chars anyway

/// Decodes a Base64-encoded string into the raw data
extern AString Base64Decode(const AString & a_Base64String);  // Exported manually due to embedded NULs and extra parameter

/// Encodes a string into Base64
extern AString Base64Encode(const AString & a_Input);  // Exported manually due to embedded NULs and extra parameter

/// Reads two bytes from the specified memory location and interprets them as BigEndian short
extern short GetBEShort(const char * a_Mem);

/// Reads four bytes from the specified memory location and interprets them as BigEndian int
extern int GetBEInt(const char * a_Mem);

/// Writes four bytes to the specified memory location so that they interpret as BigEndian int
extern void SetBEInt(char * a_Mem, Int32 a_Value);

// If you have any other string helper functions, declare them here




