
// StringUtils.h

// Interfaces to various string helper functions




#ifndef STRINGUTILS_H_INCLUDED
#define STRINGUTILS_H_INCLUDED





typedef std::string AString;
typedef std::vector<AString> AStringVector;





/// Add the formated string to the existing data in the string
extern AString & AppendVPrintf(AString & str, const char * format, va_list args);

/// Output the formatted text into the string
extern AString & Printf       (AString & str, const char * format, ...);

/// Add the formatted string to the existing data in the string
extern AString & AppendPrintf (AString & str, const char * format, ...);

/// Split the string at delimiters, return as a stringvector
extern AStringVector StringSplit(const AString & str, const AString & delim);

/// In-place string conversion to uppercase; returns the same string
extern AString & StrToUpper(AString & s);

/// Case-insensitive string comparison; returns 0 if the strings are the same
extern int NoCaseCompare(const AString & s1, const AString & s2);

/// Replaces *each* occurence of iNeedle in iHayStack with iReplaceWith
extern void ReplaceString(AString & iHayStack, const AString & iNeedle, const AString & iReplaceWith);


// If you have any other string helper functions, declare them here




#endif		// STRINGUTILS_H_INCLUDED




