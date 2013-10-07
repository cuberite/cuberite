
// HTTPMessage.cpp

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes

#include "Globals.h"
#include "HTTPMessage.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPMessage:

cHTTPMessage::cHTTPMessage(eKind a_Kind) :
	m_Kind(a_Kind),
	m_ContentLength(-1)
{
}





void cHTTPMessage::AddHeader(const AString & a_Key, const AString & a_Value)
{
	AString Key = a_Key;
	StrToLower(Key);
	cNameValueMap::iterator itr = m_Headers.find(Key);
	if (itr == m_Headers.end())
	{
		m_Headers[Key] = a_Value;
	}
	else
	{
		// The header-field key is specified multiple times, combine into comma-separated list (RFC 2616 @ 4.2)
		itr->second.append(", ");
		itr->second.append(a_Value);
	}
	
	// Special processing for well-known headers:
	if (Key == "content-type")
	{
		m_ContentType = m_Headers[Key];
	}
	else if (Key == "content-length")
	{
		m_ContentLength = atoi(m_Headers[Key].c_str());
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPRequest:

cHTTPRequest::cHTTPRequest(void) :
	super(mkRequest),
	m_EnvelopeParser(*this),
	m_IsValid(true),
	m_UserData(NULL),
	m_HasAuth(false)
{
}





int cHTTPRequest::ParseHeaders(const char * a_Data, int a_Size)
{
	if (!m_IsValid)
	{
		return -1;
	}
	
	if (m_Method.empty())
	{
		// The first line hasn't been processed yet
		int res = ParseRequestLine(a_Data, a_Size);
		if ((res < 0) || (res == a_Size))
		{
			return res;
		}
		int res2 = m_EnvelopeParser.Parse(a_Data + res, a_Size - res);
		if (res2 < 0)
		{
			m_IsValid = false;
			return res2;
		}
		return res2 + res;
	}
	
	if (m_EnvelopeParser.IsInHeaders())
	{
		int res = m_EnvelopeParser.Parse(a_Data, a_Size);
		if (res < 0)
		{
			m_IsValid = false;
		}
		return res;
	}
	return 0;
}





AString cHTTPRequest::GetBareURL(void) const
{
	size_t idxQM = m_URL.find('?');
	if (idxQM != AString::npos)
	{
		return m_URL.substr(0, idxQM);
	}
	else
	{
		return m_URL;
	}
}





int cHTTPRequest::ParseRequestLine(const char * a_Data, int a_Size)
{	
	m_IncomingHeaderData.append(a_Data, a_Size);
	size_t IdxEnd = m_IncomingHeaderData.size();

	// Ignore the initial CRLFs (HTTP spec's "should")
	size_t LineStart = 0;
	while (
		(LineStart < IdxEnd) &&
		(
			(m_IncomingHeaderData[LineStart] == '\r') ||
			(m_IncomingHeaderData[LineStart] == '\n')
		)
	)
	{
		LineStart++;
	}
	if (LineStart >= IdxEnd)
	{
		m_IsValid = false;
		return -1;
	}
	
	int NumSpaces = 0;
	size_t MethodEnd = 0;
	size_t URLEnd = 0;
	for (size_t i = LineStart; i < IdxEnd; i++)
	{
		switch (m_IncomingHeaderData[i])
		{
			case ' ':
			{
				switch (NumSpaces)
				{
					case 0:
					{
						MethodEnd = i;
						break;
					}
					case 1:
					{
						URLEnd = i;
						break;
					}
					default:
					{
						// Too many spaces in the request
						m_IsValid = false;
						return -1;
					}
				}
				NumSpaces += 1;
				break;
			}
			case '\n':
			{
				if ((i == 0) || (m_IncomingHeaderData[i - 1] != '\r') || (NumSpaces != 2) || (i < URLEnd + 7))
				{
					// LF too early, without a CR, without two preceeding spaces or too soon after the second space
					m_IsValid = false;
					return -1;
				}
				// Check that there's HTTP/version at the end
				if (strncmp(a_Data + URLEnd + 1, "HTTP/1.", 7) != 0)
				{
					m_IsValid = false;
					return -1;
				}
				m_Method = m_IncomingHeaderData.substr(LineStart, MethodEnd - LineStart);
				m_URL = m_IncomingHeaderData.substr(MethodEnd + 1, URLEnd - MethodEnd - 1);
				return i + 1;
			}
		}  // switch (m_IncomingHeaderData[i])
	}  // for i - m_IncomingHeaderData[]
	
	// CRLF hasn't been encountered yet, consider all data consumed
	return a_Size;
}





void cHTTPRequest::OnHeaderLine(const AString & a_Key, const AString & a_Value)
{
	if (
		(NoCaseCompare(a_Key, "Authorization") == 0) &&
		(strncmp(a_Value.c_str(), "Basic ", 6) == 0)
	)
	{
		AString UserPass = Base64Decode(a_Value.substr(6));
		size_t idxCol = UserPass.find(':');
		if (idxCol != AString::npos)
		{
			m_AuthUsername = UserPass.substr(0, idxCol);
			m_AuthPassword = UserPass.substr(idxCol + 1);
			m_HasAuth = true;
		}
	}
	AddHeader(a_Key, a_Value);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPResponse:

cHTTPResponse::cHTTPResponse(void) :
	super(mkResponse)
{
}





void cHTTPResponse::AppendToData(AString & a_DataStream) const
{
	a_DataStream.append("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: ");
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




