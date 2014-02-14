
// HTTPServer.cpp

// Implements the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "HTTPServer.h"
#include "HTTPMessage.h"
#include "HTTPConnection.h"
#include "HTTPFormParser.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





class cDebugCallbacks :
	public cHTTPServer::cCallbacks,
	protected cHTTPFormParser::cCallbacks
{
		virtual void OnRequestBegun(cHTTPConnection & a_Connection, cHTTPRequest & a_Request) override
		{
			if (cHTTPFormParser::HasFormData(a_Request))
			{
				a_Request.SetUserData(new cHTTPFormParser(a_Request, *this));
			}
		}
		
		
		virtual void OnRequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size) override
		{
			cHTTPFormParser * FormParser = (cHTTPFormParser *)(a_Request.GetUserData());
			if (FormParser != NULL)
			{
				FormParser->Parse(a_Data, a_Size);
			}
		}
		
		
		virtual void OnRequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request) override
		{
			cHTTPFormParser * FormParser = (cHTTPFormParser *)(a_Request.GetUserData());
			if (FormParser != NULL)
			{
				if (FormParser->Finish())
				{
					cHTTPResponse Resp;
					Resp.SetContentType("text/html");
					a_Connection.Send(Resp);
					a_Connection.Send("<html><body><table border=1 cellspacing=0><tr><th>Name</th><th>Value</th></tr>\r\n");
					for (cHTTPFormParser::iterator itr = FormParser->begin(), end = FormParser->end(); itr != end; ++itr)
					{
						a_Connection.Send(Printf("<tr><td valign=\"top\"><pre>%s</pre></td><td valign=\"top\"><pre>%s</pre></td></tr>\r\n", itr->first.c_str(), itr->second.c_str()));
					}  // for itr - FormParser[]
					a_Connection.Send("</table></body></html>");
					return;
				}
				
				// Parsing failed:
				cHTTPResponse Resp;
				Resp.SetContentType("text/plain");
				a_Connection.Send(Resp);
				a_Connection.Send("Form parsing failed");
				return;
			}
			
			// Test the auth failure and success:
			if (a_Request.GetURL() == "/auth")
			{
				if (!a_Request.HasAuth() || (a_Request.GetAuthUsername() != "a") || (a_Request.GetAuthPassword() != "b"))
				{
					a_Connection.SendNeedAuth("MCServer WebAdmin");
					return;
				}
			}
			
			cHTTPResponse Resp;
			Resp.SetContentType("text/plain");
			a_Connection.Send(Resp);
			a_Connection.Send("Hello, world");
		}
		
		
		virtual void OnFileStart(cHTTPFormParser & a_Parser, const AString & a_FileName) override
		{
			// TODO
		}
		
		
		virtual void OnFileData(cHTTPFormParser & a_Parser, const char * a_Data, int a_Size) override
		{
			// TODO
		}
		

		virtual void OnFileEnd(cHTTPFormParser & a_Parser) override
		{
			// TODO
		}
		
} g_DebugCallbacks;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHTTPServer:

cHTTPServer::cHTTPServer(void) :
	m_ListenThreadIPv4(*this, "WebServer IPv4"),
	m_ListenThreadIPv6(*this, "WebServer IPv6"),
	m_Callbacks(NULL)
{
}





cHTTPServer::~cHTTPServer()
{
	Stop();
}





bool cHTTPServer::Initialize(bool a_DualStack, const AString & a_PortsIPv4, const AString & a_PortsIPv6)
{
	bool HasAnyPort;
	m_DualStack = a_DualStack;
	if (a_DualStack)
	{
		HasAnyPort = m_ListenThreadIPv4.Initialize(cSocket::IPDual, a_PortsIPv4);
	}
	else
	{
		HasAnyPort = m_ListenThreadIPv4.Initialize(cSocket::IPv4, a_PortsIPv4);
		HasAnyPort = m_ListenThreadIPv6.Initialize(cSocket::IPv6, a_PortsIPv6) || HasAnyPort;
	}
	if (!HasAnyPort)
	{
		return false;
	}
	
	return true;
}





bool cHTTPServer::Start(cCallbacks & a_Callbacks)
{
	m_Callbacks = &a_Callbacks;
	if (!m_ListenThreadIPv4.Start())
	{
		return false;
	}
	if (!m_DualStack) 
	{
		if (!m_ListenThreadIPv6.Start())
		{
			m_ListenThreadIPv4.Stop();
			return false;
		}
	}
	return true;
}





void cHTTPServer::Stop(void)
{
	m_ListenThreadIPv4.Stop();
	if (!m_DualStack) 
	{
		m_ListenThreadIPv6.Stop();
	}
	
	// Drop all current connections:
	cCSLock Lock(m_CSConnections);
	while (!m_Connections.empty())
	{
		m_Connections.front()->Terminate();
	}  // for itr - m_Connections[]
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
	delete &a_Connection;
}





void cHTTPServer::NotifyConnectionWrite(cHTTPConnection & a_Connection)
{
	m_SocketThreads.NotifyWrite(&a_Connection);
}





void cHTTPServer::NewRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	m_Callbacks->OnRequestBegun(a_Connection, a_Request);
}





void cHTTPServer::RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size)
{
	m_Callbacks->OnRequestBody(a_Connection, a_Request, a_Data, a_Size);
}





void cHTTPServer::RequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	m_Callbacks->OnRequestFinished(a_Connection, a_Request);
	a_Connection.AwaitNextRequest();
}




