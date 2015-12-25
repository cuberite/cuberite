
// HTTPConnection.cpp

// Implements the cHTTPServerConnection class representing a single persistent connection in the HTTP server.

#include "Globals.h"
#include "HTTPServerConnection.h"
#include "HTTPRequestParser.h"
#include "HTTPServer.h"





cHTTPServerConnection::cHTTPServerConnection(cHTTPServer & a_HTTPServer) :
	m_HTTPServer(a_HTTPServer),
	m_State(wcsRecvHeaders),
	m_CurrentRequest(nullptr),
	m_CurrentRequestBodyRemaining(0)
{
	// LOGD("HTTP: New connection at %p", this);
}





cHTTPServerConnection::~cHTTPServerConnection()
{
	// LOGD("HTTP: Connection deleting: %p", this);
	delete m_CurrentRequest;
	m_CurrentRequest = nullptr;
}





void cHTTPServerConnection::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	SendData(Printf("HTTP/1.1 %d %s\r\n", a_StatusCode, a_Response.c_str()));
	SendData(Printf("Content-Length: %u\r\n\r\n", static_cast<unsigned>(a_Response.size())));
	SendData(a_Response.data(), a_Response.size());
	m_State = wcsRecvHeaders;
}





void cHTTPServerConnection::SendNeedAuth(const AString & a_Realm)
{
	SendData(Printf("HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm=\"%s\"\r\nContent-Length: 0\r\n\r\n", a_Realm.c_str()));
	m_State = wcsRecvHeaders;
}





void cHTTPServerConnection::Send(const cHTTPResponse & a_Response)
{
	ASSERT(m_State == wcsRecvIdle);
	AString toSend;
	a_Response.AppendToData(toSend);
	m_State = wcsSendingResp;
	SendData(toSend);
}





void cHTTPServerConnection::Send(const void * a_Data, size_t a_Size)
{
	ASSERT(m_State == wcsSendingResp);
	// We're sending in Chunked transfer encoding
	SendData(Printf(SIZE_T_FMT_HEX "\r\n", a_Size));
	SendData(a_Data, a_Size);
	SendData("\r\n");
}





void cHTTPServerConnection::FinishResponse(void)
{
	ASSERT(m_State == wcsSendingResp);
	SendData("0\r\n\r\n");
	m_State = wcsRecvHeaders;
}





void cHTTPServerConnection::AwaitNextRequest(void)
{
	switch (m_State)
	{
		case wcsRecvHeaders:
		{
			// Nothing has been received yet, or a special response was given (SendStatusAndReason() or SendNeedAuth())
			break;
		}

		case wcsRecvIdle:
		{
			// The client is waiting for a response, send an "Internal server error":
			SendData("HTTP/1.1 500 Internal Server Error\r\n\r\n");
			m_State = wcsRecvHeaders;
			break;
		}

		case wcsSendingResp:
		{
			// The response headers have been sent, we need to terminate the response body:
			SendData("0\r\n\r\n");
			m_State = wcsRecvHeaders;
			break;
		}

		default:
		{
			ASSERT(!"Unhandled state recovery");
			break;
		}
	}
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

	switch (m_State)
	{
		case wcsRecvHeaders:
		{
			if (m_CurrentRequest == nullptr)
			{
				m_CurrentRequest = new cHTTPRequestParser;
			}

			size_t BytesConsumed = m_CurrentRequest->ParseHeaders(a_Data, a_Size);
			if (BytesConsumed == AString::npos)
			{
				delete m_CurrentRequest;
				m_CurrentRequest = nullptr;
				m_State = wcsInvalid;
				m_Link->Close();
				m_Link.reset();
				return;
			}
			if (m_CurrentRequest->IsInHeaders())
			{
				// The request headers are not yet complete
				return;
			}

			// The request has finished parsing its headers successfully, notify of it:
			m_State = wcsRecvBody;
			m_HTTPServer.NewRequest(*this, *m_CurrentRequest);
			m_CurrentRequestBodyRemaining = m_CurrentRequest->GetContentLength();
			if (m_CurrentRequestBodyRemaining == AString::npos)
			{
				// The body length was not specified in the request, assume zero
				m_CurrentRequestBodyRemaining = 0;
			}

			// Process the rest of the incoming data into the request body:
			if (a_Size > BytesConsumed)
			{
				cHTTPServerConnection::OnReceivedData(a_Data + BytesConsumed, a_Size - BytesConsumed);
				return;
			}
			else
			{
				cHTTPServerConnection::OnReceivedData("", 0);  // If the request has zero body length, let it be processed right-away
				return;
			}
		}

		case wcsRecvBody:
		{
			ASSERT(m_CurrentRequest != nullptr);
			if (m_CurrentRequestBodyRemaining > 0)
			{
				size_t BytesToConsume = std::min(m_CurrentRequestBodyRemaining, static_cast<size_t>(a_Size));
				m_HTTPServer.RequestBody(*this, *m_CurrentRequest, a_Data, BytesToConsume);
				m_CurrentRequestBodyRemaining -= BytesToConsume;
			}
			if (m_CurrentRequestBodyRemaining == 0)
			{
				m_State = wcsRecvIdle;
				m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
				if (!m_CurrentRequest->DoesAllowKeepAlive())
				{
					m_State = wcsInvalid;
					m_Link->Close();
					m_Link.reset();
					return;
				}
				delete m_CurrentRequest;
				m_CurrentRequest = nullptr;
			}
			break;
		}

		default:
		{
			// TODO: Should we be receiving data in this state?
			break;
		}
	}
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




void cHTTPServerConnection::SendData(const void * a_Data, size_t a_Size)
{
	m_Link->Send(a_Data, a_Size);
}




