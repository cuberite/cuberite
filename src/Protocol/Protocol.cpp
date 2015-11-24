#include "Globals.h"

#include "Protocol.h"


AStringVector cProtocol::BreakApartChannels(const AString & a_Channels)
{
	// Break the string on each NUL character.
	// Note that StringSplit() doesn't work on this because NUL is a special char - string terminator
	size_t len = a_Channels.size();
	size_t first = 0;
	AStringVector res;
	for (size_t i = 0; i < len; i++)
	{
		if (a_Channels[i] != 0)
		{
			continue;
		}
		if (i > first)
		{
			res.push_back(a_Channels.substr(first, i - first));
		}
		first = i + 1;
	}  // for i - a_Channels[]
	if (first < len)
	{
		res.push_back(a_Channels.substr(first, len - first));
	}
	return res;
}
