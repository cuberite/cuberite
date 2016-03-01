
// UrlParser.h

// Declares the cUrlParser class that parses string URL into individual parts





#pragma once





class cUrlParser
{
public:
	/** Returns true if the specified scheme (http, ftp, mailto, ...) is recognized by the URL parser.
	Is case sensitive, known schemes are always lowercase. */
	static bool IsKnownScheme(const AString & a_Scheme) { return (GetDefaultPort(a_Scheme) > 0); }

	/** Returns the default port used by the specified scheme / protocol.
	If the scheme is not known, 0 is returned. */
	static UInt16 GetDefaultPort(const AString & a_Scheme);

	/** Parses the given Authority part of an URL into individual components.
	Returns true on success,
	returns false and error message on failure. */
	static std::pair<bool, AString> ParseAuthorityPart(
		const AString & a_AuthorityPart,
		AString & a_Username,
		AString & a_Password,
		AString & a_Host,
		UInt16 & a_Port
	);

	/** Parses the given URL into individual components.
	Returns true on success,
	returns false and error message on failure.
	Fails if the scheme (protocol) is not known.
	If port is missing, the default port for the specific scheme is applied. */
	static std::pair<bool, AString> Parse(
		const AString & a_Url,
		AString & a_Scheme,
		AString & a_Username,
		AString & a_Password,
		AString & a_Host,
		UInt16 & a_Port,
		AString & a_Path,
		AString & a_Query,
		AString & a_Fragment
	);
};




