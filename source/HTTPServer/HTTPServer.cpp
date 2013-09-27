
// HTTPServer.cpp

// Implements the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "HTTPServer.h"
#include "HTTPMessage.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPConnection:

cHTTPConnection::cHTTPConnection(cHTTPServer & a_HTTPServer) :
	m_HTTPServer(a_HTTPServer),
	m_State(wcsRecvHeaders),
	m_CurrentRequest(NULL)
{
}




void cHTTPConnection::SendStatusAndReason(int a_StatusCode, const AString & a_Response)
{
	AppendPrintf(m_OutgoingData, "%d %s\r\n\r\n", a_StatusCode, a_Response.c_str());
}





void cHTTPConnection::Send(const cHTTPResponse & a_Response)
{
	ASSERT(m_State = wcsRecvIdle);
	a_Response.AppendToData(m_OutgoingData);
	m_State = wcsSendingResp;
}





void cHTTPConnection::Send(const void * a_Data, int a_Size)
{
	ASSERT(m_State == wcsSendingResp);
	AppendPrintf(m_OutgoingData, "%x\r\n", a_Size);
	m_OutgoingData.append((const char *)a_Data, a_Size);
}





void cHTTPConnection::FinishResponse(void)
{
	ASSERT(m_State == wcsSendingResp);
	m_OutgoingData.append("0\r\n");
	m_State = wcsRecvHeaders;
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
			
			// Process the rest of the incoming data into the request body:
			if (m_IncomingHeaderData.size() > idxEnd + 4)
			{
				m_IncomingHeaderData.erase(0, idxEnd + 4);
				DataReceived(m_IncomingHeaderData.c_str(), m_IncomingHeaderData.size());
			}
			break;
		}

		case wcsRecvBody:
		{
			ASSERT(m_CurrentRequest != NULL);
			// TODO: Receive the body, and the next request (If HTTP/1.1 keepalive)
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPServer:

cHTTPServer::cHTTPServer(void) :
	m_ListenThreadIPv4(*this, cSocket::IPv4, "WebServer IPv4"),
	m_ListenThreadIPv6(*this, cSocket::IPv6, "WebServer IPv6"),
	m_SocketThreads()
{
}





bool cHTTPServer::Initialize(cIniFile & a_IniFile)
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





void cHTTPServer::OnConnectionAccepted(cSocket & a_Socket)
{
	cHTTPConnection * Connection = new cHTTPConnection(*this);
	m_SocketThreads.AddClient(a_Socket, Connection);
	cCSLock Lock(m_CSConnections);
	m_Connections.push_back(Connection);
}





void cHTTPServer::CloseConnection(cHTTPConnection & a_Connection)
{
	m_SocketThreads.RemoveClient(&a_Connection);
	cCSLock Lock(m_CSConnections);
	for (cHTTPConnections::iterator itr = m_Connections.begin(), end = m_Connections.end(); itr != end; ++itr)
	{
		if (*itr == &a_Connection)
		{
			m_Connections.erase(itr);
			break;
		}
	}
}





void cHTTPServer::NewRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	// TODO
}





void cHTTPServer::RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	// TODO
}





void cHTTPServer::RequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	// TODO
	
	// DEBUG: Send a debug response:
	cHTTPResponse Resp;
	Resp.SetContentType("text/plain");
	a_Connection.Send(Resp);
	a_Connection.Send("Hello");
	a_Connection.FinishResponse();
}




