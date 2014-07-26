
// HTTPConnection.cpp

// Implements the cHTTPConnection class representing a single persistent connection in the HTTP server.

#include "Globals.h"
#include "HTTPConnection.h"
#include "HTTPMessage.h"
#include "HTTPServer.h"





cHTTPConnection::cHTTPConnection(cHTTPServer & a_HTTPServer) :
	m_HTTPServer(a_HTTPServer),
	m_State(wcsRecvHeaders),
	m_CurrentRequest(NULL)
{
	// LOGD("HTTP: New connection at %p", this);
}





cHTTPConnection::~cHTTPConnection()
{
	// LOGD("HTTP: Connection deleting: %p", this);
	delete m_CurrentRequest;
	m_CurrentRequest = NULL;
}





void cHTTPConnection::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	AppendPrintf(m_OutgoingData, "%d %s\r\nContent-Length: 0\r\n\r\n", a_StatusCode, a_Response.c_str());
	m_HTTPServer.NotifyConnectionWrite(*this);
	m_State = wcsRecvHeaders;
}





void cHTTPConnection::SendNeedAuth(const AString & a_Realm)
{
	AppendPrintf(m_OutgoingData, "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm=\"%s\"\r\nContent-Length: 0\r\n\r\n", a_Realm.c_str());
	m_HTTPServer.NotifyConnectionWrite(*this);
	m_State = wcsRecvHeaders;
}





void cHTTPConnection::Send(const cHTTPResponse & a_Response)
{
	ASSERT(m_State == wcsRecvIdle);
	a_Response.AppendToData(m_OutgoingData);
	m_State = wcsSendingResp;
	m_HTTPServer.NotifyConnectionWrite(*this);
}





void cHTTPConnection::Send(const void * a_Data, size_t a_Size)
{
	ASSERT(m_State == wcsSendingResp);
	AppendPrintf(m_OutgoingData, SIZE_T_FMT_HEX "\r\n", a_Size);
	m_OutgoingData.append((const char *)a_Data, a_Size);
	m_OutgoingData.append("\r\n");
	m_HTTPServer.NotifyConnectionWrite(*this);
}





void cHTTPConnection::FinishResponse(void)
{
	ASSERT(m_State == wcsSendingResp);
	m_OutgoingData.append("0\r\n\r\n");
	m_State = wcsRecvHeaders;
	m_HTTPServer.NotifyConnectionWrite(*this);
}





void cHTTPConnection::AwaitNextRequest(void)
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
			m_OutgoingData.append("HTTP/1.1 500 Internal Server Error\r\n\r\n");
			m_HTTPServer.NotifyConnectionWrite(*this);
			m_State = wcsRecvHeaders;
			break;
		}
		
		case wcsSendingResp:
		{
			// The response headers have been sent, we need to terminate the response body:
			m_OutgoingData.append("0\r\n\r\n");
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





void cHTTPConnection::Terminate(void)
{
	if (m_CurrentRequest != NULL)
	{
		m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
	}
	m_HTTPServer.CloseConnection(*this);
}





bool cHTTPConnection::DataReceived(const char * a_Data, size_t a_Size)
{
	switch (m_State)
	{
		case wcsRecvHeaders:
		{
			if (m_CurrentRequest == NULL)
			{
				m_CurrentRequest = new cHTTPRequest;
			}

			size_t BytesConsumed = m_CurrentRequest->ParseHeaders(a_Data, a_Size);
			if (BytesConsumed == AString::npos)
			{
				delete m_CurrentRequest;
				m_CurrentRequest = NULL;
				m_State = wcsInvalid;
				m_HTTPServer.CloseConnection(*this);
				return true;
			}
			if (m_CurrentRequest->IsInHeaders())
			{
				// The request headers are not yet complete
				return false;
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
				return cHTTPConnection::DataReceived(a_Data + BytesConsumed, a_Size - BytesConsumed);
			}
			else
			{
				return cHTTPConnection::DataReceived("", 0);  // If the request has zero body length, let it be processed right-away
			}
		}

		case wcsRecvBody:
		{
			ASSERT(m_CurrentRequest != NULL);
			if (m_CurrentRequestBodyRemaining > 0)
			{
				size_t BytesToConsume = std::min(m_CurrentRequestBodyRemaining, (size_t)a_Size);
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
					m_HTTPServer.CloseConnection(*this);
					return true;
				}
				delete m_CurrentRequest;
				m_CurrentRequest = NULL;
			}
			break;
		}

		default:
		{
			// TODO: Should we be receiving data in this state?
			break;
		}
	}
	return false;
}





void cHTTPConnection::GetOutgoingData(AString & a_Data)
{
	std::swap(a_Data, m_OutgoingData);
}





void cHTTPConnection::SocketClosed(void)
{
	if (m_CurrentRequest != NULL)
	{
		m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
	}
	m_HTTPServer.CloseConnection(*this);
}





