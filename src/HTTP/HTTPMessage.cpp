
// HTTPMessage.cpp

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes

#include "Globals.h"
#include "HTTPMessage.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





////////////////////////////////////////////////////////////////////////////////
// cHTTPMessage:

cHTTPMessage::cHTTPMessage(eKind a_Kind) :
	m_Kind(a_Kind),
	m_ContentLength(AString::npos)
{
}





void cHTTPMessage::AddHeader(const AString & a_Key, const AString & a_Value)
{
	auto Key = StrToLower(a_Key);
	auto itr = m_Headers.find(Key);
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
		if (!StringToInteger(m_Headers[Key], m_ContentLength))
		{
			m_ContentLength = 0;
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cHTTPOutgoingResponse:

cHTTPOutgoingResponse::cHTTPOutgoingResponse(void) :
	Super(mkResponse)
{
}





void cHTTPOutgoingResponse::AppendToData(AString & a_DataStream) const
{
	a_DataStream.append("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: ");
	a_DataStream.append(m_ContentType);
	a_DataStream.append("\r\n");
	for (auto itr = m_Headers.cbegin(), end = m_Headers.cend(); itr != end; ++itr)
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





////////////////////////////////////////////////////////////////////////////////
// cHTTPIncomingRequest:

cHTTPIncomingRequest::cHTTPIncomingRequest(const AString & a_Method, const AString & a_URL):
	Super(mkRequest),
	m_Method(a_Method),
	m_URL(a_URL),
	m_HasAuth(false)
{
}





AString cHTTPIncomingRequest::GetURLPath(void) const
{
	auto idxQuestionMark = m_URL.find('?');
	if (idxQuestionMark == AString::npos)
	{
		return m_URL;
	}
	else
	{
		return m_URL.substr(0, idxQuestionMark);
	}
}





void cHTTPIncomingRequest::AddHeader(const AString & a_Key, const AString & a_Value)
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
	Super::AddHeader(a_Key, a_Value);
}




