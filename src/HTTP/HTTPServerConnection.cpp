
// HTTPConnection.cpp

// Implements the cHTTPServerConnection class representing a single persistent connection in the HTTP server.

#include "Globals.h"
#include "HTTPServerConnection.h"
#include "HTTPMessage.h"
#include "HTTPServer.h"





cHTTPServerConnection::cHTTPServerConnection(cHTTPServer & a_HTTPServer) :
	m_HTTPServer(a_HTTPServer),
	m_Parser(*this),
	m_CurrentRequest(nullptr)
{
	// LOGD("HTTP: New connection at %p", this);
}





cHTTPServerConnection::~cHTTPServerConnection()
{
	// LOGD("HTTP: Connection deleting: %p", this);
	m_CurrentRequest.reset();
}





void cHTTPServerConnection::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	SendData(Printf("HTTP/1.1 %d %s\r\n", a_StatusCode, a_Response.c_str()));
	SendData(Printf("Content-Length: %u\r\n\r\n", static_cast<unsigned>(a_Response.size())));
	SendData(a_Response.data(), a_Response.size());
	m_CurrentRequest.reset();
	m_Parser.Reset();
}





void cHTTPServerConnection::SendNeedAuth(const AString & a_Realm)
{
	SendData(Printf("HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm=\"%s\"\r\nContent-Length: 0\r\n\r\n", a_Realm.c_str()));
	m_CurrentRequest.reset();
	m_Parser.Reset();
}





void cHTTPServerConnection::Send(const cHTTPOutgoingResponse & a_Response)
{
	ASSERT(m_CurrentRequest != nullptr);
	AString toSend;
	a_Response.AppendToData(toSend);
	SendData(toSend);
}





void cHTTPServerConnection::Send(const void * a_Data, size_t a_Size)
{
	ASSERT(m_CurrentRequest != nullptr);
	// We're sending in Chunked transfer encoding
	SendData(fmt::format("{0:x}\r\n", a_Size));
	SendData(a_Data, a_Size);
	SendData("\r\n");
}





void cHTTPServerConnection::FinishResponse(void)
{
	ASSERT(m_CurrentRequest != nullptr);
	SendData("0\r\n\r\n");
	m_CurrentRequest.reset();
	m_Parser.Reset();
}





void cHTTPServerConnection::Terminate(void)
{
	if (m_CurrentRequest != nullptr)
	{
		m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
	}
	m_Link.reset();
}





void cHTTPServerConnection::OnLinkCreated(cTCPLinkPtr a_Link)
{
	ASSERT(m_Link == nullptr);
	m_Link = a_Link;
}





void cHTTPServerConnection::OnReceivedData(const char * a_Data, size_t a_Size)
{
	ASSERT(m_Link != nullptr);

	m_Parser.Parse(a_Data, a_Size);
}





void cHTTPServerConnection::OnRemoteClosed(void)
{
	if (m_CurrentRequest != nullptr)
	{
		m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
	}
	m_Link.reset();
}






void cHTTPServerConnection::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	OnRemoteClosed();
}





void cHTTPServerConnection::OnError(const AString & a_ErrorDescription)
{
	OnRemoteClosed();
}





void cHTTPServerConnection::OnFirstLine(const AString & a_FirstLine)
{
	// Create a new request object for this request:
	auto split = StringSplit(a_FirstLine, " ");
	if (split.size() < 2)
	{
		// Invalid request line. We need at least the Method and URL
		OnRemoteClosed();
		return;
	}
	m_CurrentRequest.reset(new cHTTPIncomingRequest(split[0], split[1]));
}





void cHTTPServerConnection::OnHeaderLine(const AString & a_Key, const AString & a_Value)
{
	if (m_CurrentRequest == nullptr)
	{
		return;
	}
	m_CurrentRequest->AddHeader(a_Key, a_Value);
}





void cHTTPServerConnection::OnHeadersFinished(void)
{
	if (m_CurrentRequest == nullptr)
	{
		return;
	}
	m_HTTPServer.NewRequest(*this, *m_CurrentRequest);
}





void cHTTPServerConnection::OnBodyData(const void * a_Data, size_t a_Size)
{
	if (m_CurrentRequest == nullptr)
	{
		return;
	}
	m_HTTPServer.RequestBody(*this, *m_CurrentRequest, a_Data, a_Size);
}





void cHTTPServerConnection::OnBodyFinished(void)
{
	// Process the request and reset:
	m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
	m_CurrentRequest.reset();
	m_Parser.Reset();
}





void cHTTPServerConnection::SendData(const void * a_Data, size_t a_Size)
{
	m_Link->Send(a_Data, a_Size);
}




