
// HTTPServer.cpp

// Implements the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "HTTPServer.h"
#include "HTTPMessage.h"
#include "HTTPConnection.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





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





void cHTTPServer::NotifyConnectionWrite(cHTTPConnection & a_Connection)
{
	m_SocketThreads.NotifyWrite(&a_Connection);
}





void cHTTPServer::NewRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	// TODO
}





void cHTTPServer::RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size)
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




