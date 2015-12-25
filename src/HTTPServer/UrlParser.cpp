
// UrlParser.cpp

// Implements the cUrlParser class that parses string URL into individual parts

#include "Globals.h"
#include "UrlParser.h"





UInt16 cUrlParser::GetDefaultPort(const AString & a_Scheme)
{
	if (a_Scheme == "http")
	{
		return 80;
	}
	else if (a_Scheme == "https")
	{
		return 443;
	}
	else if (a_Scheme == "ftp")
	{
		return 21;
	}
	else if (a_Scheme == "mailto")
	{
		return 25;
	}
	return 0;
}





std::pair<bool, AString> cUrlParser::ParseAuthorityPart(
	const AString & a_AuthorityPart,
	AString & a_Username,
	AString & a_Password,
	AString & a_Host,
	UInt16 & a_Port
)
{
	/*
	a_AuthorityPart format:
	[user:password@]host[:port]
	host can be an IPv4, hostname, or an IPv6 enclosed in brackets
	Assume only the password can contain an additional at-sign
	*/

	// Split the authority on the last at-sign, if present:
	auto idxLastAtSign = a_AuthorityPart.find_last_of('@');
	auto credPart = (idxLastAtSign == AString::npos) ? AString() : a_AuthorityPart.substr(0, idxLastAtSign);
	auto srvrPart = (idxLastAtSign == AString::npos) ? a_AuthorityPart : a_AuthorityPart.substr(idxLastAtSign + 1);

	// User credentials are completely optional:
	auto idxCredColon = credPart.find(':');
	a_Username = credPart.substr(0, idxCredColon);
	a_Password = (idxCredColon == AString::npos) ? AString() : credPart.substr(idxCredColon + 1);

	// Host can be a hostname, IPv4 or [IPv6]. If in brackets, search for the closing bracket first
	if (srvrPart.empty())
	{
		// No host information at all. Bail out with success
		a_Host.clear();
		return std::make_pair(true, AString());
	}
	if (srvrPart[0] == '[')
	{
		// [IPv6] host, search for the closing bracket
		auto idxClosingBracket = srvrPart.find(']');
		if (idxClosingBracket == AString::npos)
		{
			return std::make_pair(false, "Invalid IPv6-like address, missing closing bracket");
		}
		a_Host = srvrPart.substr(0, idxClosingBracket);
		auto portPart = srvrPart.substr(idxClosingBracket + 1);
		if (portPart.empty())
		{
			// No port was specified, return success
			return std::make_pair(true, AString());
		}
		if (portPart[0] != ':')
		{
			return std::make_pair(false, "Invalid port format after IPv6 address, mising colon");
		}
		if (!StringToInteger(portPart.substr(2), a_Port))
		{
			return std::make_pair(false, "Failed to parse port number after IPv6 address");
		}
		return std::make_pair(true, AString());
	}

	// Not an [IPv6] address, split on the last colon:
	auto idxLastColon = srvrPart.find_last_of(':');
	a_Host = srvrPart.substr(0, idxLastColon);
	if (idxLastColon == AString::npos)
	{
		// No port was specified, return success
		return std::make_pair(true, AString());
	}
	auto portPart = srvrPart.substr(idxLastColon + 1);
	if (!StringToInteger(portPart, a_Port))
	{
		return std::make_pair(false, "Failed to parse port number after hostname");
	}
	return std::make_pair(true, AString());
}





std::pair<bool, AString> cUrlParser::Parse(
	const AString & a_Url,
	AString & a_Scheme,
	AString & a_Username,
	AString & a_Password,
	AString & a_Host,
	UInt16 & a_Port,
	AString & a_Path,
	AString & a_Query,
	AString & a_Fragment
)
{
	// Find the scheme - the text before the first colon:
	auto idxColon = a_Url.find(':');
	if (idxColon == AString::npos)
	{
		return std::make_pair(false, "Cannot parse the Scheme part of the URL");
	}
	a_Scheme = StrToLower(a_Url.substr(0, idxColon));
	a_Port = GetDefaultPort(a_Scheme);
	if (a_Port == 0)
	{
		return std::make_pair(false, Printf("Unknown URL scheme: \"%s\"", a_Scheme.c_str()));
	}

	// If the next two chars are a double-slash, skip them:
	auto authStart = idxColon + 1;
	if (a_Url.substr(authStart, 2) == "//")
	{
		authStart += 2;
	}

	// The Authority part follows the Scheme, until the first slash:
	auto idxFirstSlash = a_Url.find('/', authStart + 1);
	if (idxFirstSlash == AString::npos)
	{
		// No slash, the whole end of the Url is the authority part
		idxFirstSlash = a_Url.size();
	}

	// Parse the Authority part into individual components:
	auto res = ParseAuthorityPart(
		a_Url.substr(authStart, idxFirstSlash - authStart),
		a_Username, a_Password,
		a_Host, a_Port
	);
	if (!res.first)
	{
		return res;
	}

	// Parse the rest into a path, query and fragment:
	a_Path.clear();
	a_Query.clear();
	a_Fragment.clear();
	if (idxFirstSlash == a_Url.size())
	{
		// No additional data, bail out with success
		return std::make_pair(true, AString());
	}
	auto idxPathEnd = a_Url.find_first_of("?#", idxFirstSlash + 1);
	if (idxPathEnd == AString::npos)
	{
		a_Path = a_Url.substr(idxFirstSlash);
		return std::make_pair(true, AString());
	}
	a_Path = a_Url.substr(idxFirstSlash, idxPathEnd - idxFirstSlash);
	auto idxHash = a_Url.find('#', idxPathEnd);
	if (idxHash == AString::npos)
	{
		a_Query = a_Url.substr(idxPathEnd + 1);
		return std::make_pair(true, AString());
	}
	if (idxHash > idxPathEnd)
	{
		a_Query = a_Url.substr(idxPathEnd + 1, idxHash - idxPathEnd - 1);
	}
	a_Fragment = a_Url.substr(idxHash + 1);
	return std::make_pair(true, AString());
}





