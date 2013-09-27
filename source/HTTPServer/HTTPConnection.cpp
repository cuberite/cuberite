
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
}




void cHTTPConnection::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	AppendPrintf(m_OutgoingData, "%d %s\r\n\r\n", a_StatusCode, a_Response.c_str());
	m_HTTPServer.NotifyConnectionWrite(*this);
}





void cHTTPConnection::Send(const cHTTPResponse & a_Response)
{
	ASSERT(m_State = wcsRecvIdle);
	a_Response.AppendToData(m_OutgoingData);
	m_State = wcsSendingResp;
	m_HTTPServer.NotifyConnectionWrite(*this);
}





void cHTTPConnection::Send(const void * a_Data, int a_Size)
{
	ASSERT(m_State == wcsSendingResp);
	AppendPrintf(m_OutgoingData, "%x\r\n", a_Size);
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





void cHTTPConnection::DataReceived(const char * a_Data, int a_Size)
{
	switch (m_State)
	{
		case wcsRecvHeaders:
		{
			ASSERT(m_CurrentRequest == NULL);

			// Start searching 3 chars from the end of the already received data, if available:
			size_t SearchStart = m_IncomingHeaderData.size();
			SearchStart = (SearchStart > 3) ? SearchStart - 3 : 0;
			
			m_IncomingHeaderData.append(a_Data, a_Size);
			
			// Parse the header, if it is complete:
			size_t idxEnd = m_IncomingHeaderData.find("\r\n\r\n", SearchStart);
			if (idxEnd == AString::npos)
			{
				return;
			}
			m_CurrentRequest = new cHTTPRequest;
			if (!m_CurrentRequest->ParseHeaders(m_IncomingHeaderData.c_str(), idxEnd + 2))
			{
				delete m_CurrentRequest;
				m_CurrentRequest = NULL;
				m_State = wcsInvalid;
				m_HTTPServer.CloseConnection(*this);
				return;
			}
			m_State = wcsRecvBody;
			m_HTTPServer.NewRequest(*this, *m_CurrentRequest);
			m_CurrentRequestBodyRemaining = m_CurrentRequest->GetContentLength();
			
			// Process the rest of the incoming data into the request body:
			if (m_IncomingHeaderData.size() > idxEnd + 4)
			{
				m_IncomingHeaderData.erase(0, idxEnd + 4);
				DataReceived(m_IncomingHeaderData.c_str(), m_IncomingHeaderData.size());
				m_IncomingHeaderData.clear();
			}
			else
			{
				m_IncomingHeaderData.clear();
				DataReceived("", 0);  // If the request has zero body length, let it be processed right-away
			}
			break;
		}

		case wcsRecvBody:
		{
			ASSERT(m_CurrentRequest != NULL);
			if (m_CurrentRequestBodyRemaining > 0)
			{
				int BytesToConsume = std::min(m_CurrentRequestBodyRemaining, a_Size);
				m_HTTPServer.RequestBody(*this, *m_CurrentRequest, a_Data, BytesToConsume);
				m_CurrentRequestBodyRemaining -= BytesToConsume;
			}
			if (m_CurrentRequestBodyRemaining == 0)
			{
				m_HTTPServer.RequestFinished(*this, *m_CurrentRequest);
				m_State = wcsRecvIdle;
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
}





