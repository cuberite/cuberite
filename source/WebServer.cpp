
// WebServer.cpp

// Implements the cWebServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "WebServer.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWebRequest:

cWebRequest::cWebRequest(cWebServer & a_WebServer) :
	m_WebServer(a_WebServer),
	m_IsReceivingHeaders(true)
{
}




void cWebRequest::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	AppendPrintf(m_OutgoingData, "%d %s\r\n\r\n", a_StatusCode, a_Response.c_str());
}





void cWebRequest::ParseHeader(size_t a_IdxEnd)
{
	size_t Next = ParseRequestLine(a_IdxEnd);
	if (Next == AString::npos)
	{
		SendStatusAndReason(HTTP_BAD_REQUEST, "Bad request");
		return;
	}
	
	AString Key;
	while (Next < a_IdxEnd)
	{
		Next = ParseHeaderField(Next, a_IdxEnd, Key);
		if (Next == AString::npos)
		{
			SendStatusAndReason(HTTP_BAD_REQUEST, "Bad request");
			return;
		}
	}
	
	m_WebServer.RequestReady(this);
}




size_t cWebRequest::ParseRequestLine(size_t a_IdxEnd)
{	
	// Ignore the initial CRLFs (HTTP spec's "should")
	size_t LineStart = 0;
	while (
		(LineStart < a_IdxEnd) &&
		(
			(m_IncomingHeaderData[LineStart] == '\r') ||
			(m_IncomingHeaderData[LineStart] == '\n')
		)
	)
	{
		LineStart++;
	}
	if (LineStart >= a_IdxEnd)
	{
		return AString::npos;
	}
	
	// Get the Request-Line
	size_t LineEnd = m_IncomingHeaderData.find("\r\n", LineStart);
	if (LineEnd == AString::npos)
	{
		return AString::npos;
	}
	AString RequestLine = m_IncomingHeaderData.substr(LineStart, LineEnd - LineStart);
	
	// Find the method:
	size_t Space = RequestLine.find(" ", LineStart);
	if (Space == AString::npos)
	{
		return AString::npos;
	}
	m_Method = RequestLine.substr(0, Space);
	
	// Find the URL:
	size_t Space2 = RequestLine.find(" ", Space + 1);
	if (Space2 == AString::npos)
	{
		return AString::npos;
	}
	m_URL = RequestLine.substr(Space, Space2 - Space);
	
	// Check that there's HTTP/version at the end
	if (strncmp(RequestLine.c_str() + Space2 + 1, "HTTP/1.", 7) != 0)
	{
		return AString::npos;
	}
	
	return LineEnd + 2;
}





size_t cWebRequest::ParseHeaderField(size_t a_IdxStart, size_t a_IdxEnd, AString & a_Key)
{
	if (a_IdxStart >= a_IdxEnd)
	{
		return a_IdxEnd;
	}
	if (m_IncomingHeaderData[a_IdxStart] <= ' ')
	{
		return ParseHeaderFieldContinuation(a_IdxStart + 1, a_IdxEnd, a_Key);
	}
	size_t ValueIdx = 0;
	AString Key;
	for (size_t i = a_IdxStart; i < a_IdxEnd; i++)
	{
		switch (m_IncomingHeaderData[i])
		{
			case '\n':
			{
				if ((ValueIdx == 0) || (i < ValueIdx - 2) || (i < a_IdxStart + 1) || (m_IncomingHeaderData[i - 1] != '\r'))
				{
					// Invalid header field - no colon or no CR before LF
					return AString::npos;
				}
				AString Value = m_IncomingHeaderData.substr(ValueIdx + 1, i - ValueIdx - 2);
				AddHeader(Key, Value);
				a_Key = Key;
				return i + 1;
			}
			case ':':
			{
				if (ValueIdx == 0)
				{
					Key = m_IncomingHeaderData.substr(a_IdxStart, i - a_IdxStart);
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





size_t cWebRequest::ParseHeaderFieldContinuation(size_t a_IdxStart, size_t a_IdxEnd, AString & a_Key)
{
	size_t Start = a_IdxStart;
	for (size_t i = a_IdxStart; i < a_IdxEnd; i++)
	{
		if ((m_IncomingHeaderData[i] > ' ') && (Start == a_IdxStart))
		{
			Start = i;
		}
		else if (m_IncomingHeaderData[i] == '\n')
		{
			if ((i < a_IdxStart + 1) || (m_IncomingHeaderData[i - 1] != '\r'))
			{
				// There wasn't a CR before this LF
				return AString::npos;
			}
			AString Value = m_IncomingHeaderData.substr(Start, i - Start - 1);
			AddHeader(a_Key, Value);
			return i + 1;
		}
	}
	// LF not found, how? We found it at the header end (CRLFCRLF)
	ASSERT(!"LF not found, wtf?");
	return AString::npos;
}





void cWebRequest::AddHeader(const AString & a_Key, const AString & a_Value)
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
}





void cWebRequest::DataReceived(const char * a_Data, int a_Size)
{
	if (m_IsReceivingHeaders)
	{
		// Start searching 3 chars from the end of the already received data, if available:
		size_t SearchStart = m_IncomingHeaderData.size();
		SearchStart = (SearchStart > 3) ? SearchStart - 3 : 0;
		
		m_IncomingHeaderData.append(a_Data, a_Size);
		
		// Parse the header, if it is complete:
		size_t idxEnd = m_IncomingHeaderData.find("\r\n\r\n", SearchStart);
		if (idxEnd != AString::npos)
		{
			ParseHeader(idxEnd + 2);
			m_IsReceivingHeaders = false;
		}
	}
	else
	{
		// TODO: Receive the body, and the next request (If HTTP/1.1 keepalive
	}
}





void cWebRequest::GetOutgoingData(AString & a_Data)
{
	std::swap(a_Data, m_OutgoingData);
}





void cWebRequest::SocketClosed(void)
{
	// TODO: m_WebServer.RequestFinished(this);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWebServer:

cWebServer::cWebServer(void) :
	m_ListenThreadIPv4(*this, cSocket::IPv4, "WebServer IPv4"),
	m_ListenThreadIPv6(*this, cSocket::IPv6, "WebServer IPv6"),
	m_SocketThreads()
{
}





bool cWebServer::Initialize(cIniFile & a_IniFile)
{
	if (!a_IniFile.GetValueSetB("WebAdmin", "Enabled", false))
	{
		// The WebAdmin is disabled
		return true;
	}
	bool HasAnyPort;
	HasAnyPort = m_ListenThreadIPv4.Initialize(a_IniFile.GetValueSet("WebAdmin", "Port", "8081"));
	HasAnyPort = m_ListenThreadIPv6.Initialize(a_IniFile.GetValueSet("WebAdmin", "PortsIPv6", "8082, 3300")) || HasAnyPort;
	if (!HasAnyPort)
	{
		LOG("WebAdmin is disabled");
		return false;
	}
	if (!m_ListenThreadIPv4.Start())
	{
		return false;
	}
	if (!m_ListenThreadIPv6.Start())
	{
		m_ListenThreadIPv4.Stop();
		return false;
	}
	return true;
}





void cWebServer::OnConnectionAccepted(cSocket & a_Socket)
{
	cWebRequest * Request = new cWebRequest(*this);
	m_SocketThreads.AddClient(a_Socket, Request);
	cCSLock Lock(m_CSRequests);
	m_Requests.push_back(Request);
}





void cWebServer::RequestReady(cWebRequest * a_Request)
{
	a_Request->SendStatusAndReason(cWebRequest::HTTP_OK, "Hello");
}




