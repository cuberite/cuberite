
// HTTPRequestParser.cpp

// Implements the cHTTPRequestParser class representing the parser for incoming HTTP requests

#include "Globals.h"
#include "HTTPRequestParser.h"





cHTTPRequestParser::cHTTPRequestParser(void) :
	super(mkRequest),
	m_EnvelopeParser(*this),
	m_IsValid(true),
	m_UserData(nullptr),
	m_HasAuth(false),
	m_AllowKeepAlive(false)
{
}





size_t cHTTPRequestParser::ParseHeaders(const char * a_Data, size_t a_Size)
{
	if (!m_IsValid)
	{
		return AString::npos;
	}
	
	if (m_Method.empty())
	{
		// The first line hasn't been processed yet
		size_t res = ParseRequestLine(a_Data, a_Size);
		if ((res == AString::npos) || (res == a_Size))
		{
			return res;
		}
		size_t res2 = m_EnvelopeParser.Parse(a_Data + res, a_Size - res);
		if (res2 == AString::npos)
		{
			m_IsValid = false;
			return res2;
		}
		return res2 + res;
	}
	
	if (m_EnvelopeParser.IsInHeaders())
	{
		size_t res = m_EnvelopeParser.Parse(a_Data, a_Size);
		if (res == AString::npos)
		{
			m_IsValid = false;
		}
		return res;
	}
	return 0;
}





AString cHTTPRequestParser::GetBareURL(void) const
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





size_t cHTTPRequestParser::ParseRequestLine(const char * a_Data, size_t a_Size)
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
		return AString::npos;
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
						return AString::npos;
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
					return AString::npos;
				}
				// Check that there's HTTP / version at the end
				if (strncmp(m_IncomingHeaderData.c_str() + URLEnd + 1, "HTTP/1.", 7) != 0)
				{
					m_IsValid = false;
					return AString::npos;
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





void cHTTPRequestParser::OnHeaderLine(const AString & a_Key, const AString & a_Value)
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
	if ((a_Key == "Connection") && (NoCaseCompare(a_Value, "keep-alive") == 0))
	{
		m_AllowKeepAlive = true;
	}
	AddHeader(a_Key, a_Value);
}




