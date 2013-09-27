
// HTTPMessage.cpp

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes

#include "Globals.h"
#include "HTTPMessage.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPMessage:

cHTTPMessage::cHTTPMessage(eKind a_Kind) :
	m_Kind(a_Kind)
{
}





void cHTTPMessage::AddHeader(const AString & a_Key, const AString & a_Value)
{
	cNameValueMap::iterator itr = m_Headers.find(a_Key);
	if (itr == m_Headers.end())
	{
		m_Headers[a_Key] = a_Value;
	}
	else
	{
		// The header-field key is specified multiple times, combine into comma-separated list (RFC 2616 @ 4.2)
		itr->second.append(", ");
		itr->second.append(a_Value);
	}
	
	// Special processing for well-known headers:
	if (a_Key == "Content-Type")
	{
		m_ContentType = m_Headers["Content-Type"];
	}
	else if (a_Key == "Content-Length")
	{
		m_ContentLength = atoi(m_Headers["Content-Length"].c_str());
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPRequest:

cHTTPRequest::cHTTPRequest(void) :
	super(mkRequest)
{
}





bool cHTTPRequest::ParseHeaders(const char * a_IncomingData, size_t a_IdxEnd)
{
	// The first line contains the method and the URL:
	size_t Next = ParseRequestLine(a_IncomingData, a_IdxEnd);
	if (Next == AString::npos)
	{
		return false;
	}
	
	// The following lines contain headers:
	AString Key;
	const char * Data = a_IncomingData + Next;
	size_t End = a_IdxEnd - Next;
	while (End > 0)
	{
		Next = ParseHeaderField(Data, End, Key);
		if (Next == AString::npos)
		{
			return false;
		}
		ASSERT(End >= Next);
		Data += Next;
		End -= Next;
	}
	
	return HasReceivedContentLength();
}




size_t cHTTPRequest::ParseRequestLine(const char * a_Data, size_t a_IdxEnd)
{	
	// Ignore the initial CRLFs (HTTP spec's "should")
	size_t LineStart = 0;
	while (
		(LineStart < a_IdxEnd) &&
		(
			(a_Data[LineStart] == '\r') ||
			(a_Data[LineStart] == '\n')
		)
	)
	{
		LineStart++;
	}
	if (LineStart >= a_IdxEnd)
	{
		return AString::npos;
	}
	
	size_t Last = LineStart;
	int NumSpaces = 0;
	for (size_t i = LineStart; i < a_IdxEnd; i++)
	{
		switch (a_Data[i])
		{
			case ' ':
			{
				switch (NumSpaces)
				{
					case 0:
					{
						m_Method.assign(a_Data, Last, i - Last - 1);
						break;
					}
					case 1:
					{
						m_URL.assign(a_Data, Last, i - Last - 1);
						break;
					}
					default:
					{
						// Too many spaces in the request
						return AString::npos;
					}
				}
				Last = i + 1;
				NumSpaces += 1;
				break;
			}
			case '\n':
			{
				if ((i == 0) || (a_Data[i] != '\r') || (NumSpaces != 2) || (i < Last + 7))
				{
					// LF too early, without a CR, without two preceeding spaces or too soon after the second space
					return AString::npos;
				}
				// Check that there's HTTP/version at the end
				if (strncmp(a_Data + Last, "HTTP/1.", 7) != 0)
				{
					return AString::npos;
				}
				return i;
			}
		}  // switch (a_Data[i])
	}  // for i - a_Data[]
	return AString::npos;
}





size_t cHTTPRequest::ParseHeaderField(const char * a_Data, size_t a_IdxEnd, AString & a_Key)
{
	if (*a_Data <= ' ')
	{
		size_t res = ParseHeaderFieldContinuation(a_Data + 1, a_IdxEnd - 1, a_Key);
		return (res == AString::npos) ? res : (res + 1);
	}
	size_t ValueIdx = 0;
	AString Key;
	for (size_t i = 0; i < a_IdxEnd; i++)
	{
		switch (a_Data[i])
		{
			case '\n':
			{
				if ((ValueIdx == 0) || (i < ValueIdx - 2) || (i == 0) || (a_Data[i - 1] != '\r'))
				{
					// Invalid header field - no colon or no CR before LF
					return AString::npos;
				}
				AString Value(a_Data, ValueIdx + 1, i - ValueIdx - 2);
				AddHeader(Key, Value);
				a_Key = Key;
				return i + 1;
			}
			case ':':
			{
				if (ValueIdx == 0)
				{
					Key.assign(a_Data, 0, i);
					ValueIdx = i;
				}
				break;
			}
			case ' ':
			case '\t':
			{
				if (ValueIdx == i - 1)
				{
					// Value has started in this char, but it is whitespace, so move the start one char further
					ValueIdx = i;
				}
			}
		}  // switch (char)
	}  // for i - m_IncomingHeaderData[]
	// No header found, return the end-of-data index:
	return a_IdxEnd;
}





size_t cHTTPRequest::ParseHeaderFieldContinuation(const char * a_Data, size_t a_IdxEnd, AString & a_Key)
{
	size_t Start = 0;
	for (size_t i = 0; i < a_IdxEnd; i++)
	{
		if ((a_Data[i] > ' ') && (Start == 0))
		{
			Start = i;
		}
		else if (a_Data[i] == '\n')
		{
			if ((i == 0) || (a_Data[i - 1] != '\r'))
			{
				// There wasn't a CR before this LF
				return AString::npos;
			}
			AString Value(a_Data, 0, i - Start - 1);
			AddHeader(a_Key, Value);
			return i + 1;
		}
	}
	// LF not found, how? We found it at the header end (CRLFCRLF)
	ASSERT(!"LF not found, wtf?");
	return AString::npos;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPResponse:

cHTTPResponse::cHTTPResponse(void) :
	super(mkResponse)
{
}





void cHTTPResponse::AppendToData(AString & a_DataStream) const
{
	a_DataStream.append("200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: ");
	a_DataStream.append(m_ContentType);
	a_DataStream.append("\r\n");
	for (cNameValueMap::const_iterator itr = m_Headers.begin(), end = m_Headers.end(); itr != end; ++itr)
	{
		if ((itr->first == "Content-Type") || (itr->first == "Content-Length"))
		{
			continue;
		}
		a_DataStream.append(itr->first);
		a_DataStream.append(": ");
		a_DataStream.append(itr->second);
		a_DataStream.append("\r\n");
	}  // for itr - m_Headers[]
	a_DataStream.append("\r\n");
}




