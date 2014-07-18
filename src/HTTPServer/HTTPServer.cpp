
// HTTPServer.cpp

// Implements the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "HTTPServer.h"
#include "HTTPMessage.h"
#include "HTTPConnection.h"
#include "HTTPFormParser.h"
#include "SslHTTPConnection.h"





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
			UNUSED(a_Connection);
			
			if (cHTTPFormParser::HasFormData(a_Request))
			{
				a_Request.SetUserData(new cHTTPFormParser(a_Request, *this));
			}
		}
		
		
		virtual void OnRequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, size_t a_Size) override
		{
			UNUSED(a_Connection);
			
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
		
		
		virtual void OnFileData(cHTTPFormParser & a_Parser, const char * a_Data, size_t a_Size) override
		{
			// TODO
		}
		

		virtual void OnFileEnd(cHTTPFormParser & a_Parser) override
		{
			// TODO
		}
		
} g_DebugCallbacks;





////////////////////////////////////////////////////////////////////////////////
// cHTTPServer:

cHTTPServer::cHTTPServer(void) :
	m_ListenThreadIPv4(*this, cSocket::IPv4, "WebServer"),
	m_ListenThreadIPv6(*this, cSocket::IPv6, "WebServer"),
	m_Callbacks(NULL)
{
}





cHTTPServer::~cHTTPServer()
{
	Stop();
}





bool cHTTPServer::Initialize(const AString & a_PortsIPv4, const AString & a_PortsIPv6)
{
	// Read the HTTPS cert + key:
	AString CertFile = cFile::ReadWholeFile("webadmin/httpscert.crt");
	AString KeyFile  = cFile::ReadWholeFile("webadmin/httpskey.pem");
	if (!CertFile.empty() && !KeyFile.empty())
	{
		m_Cert.reset(new cX509Cert);
		int res = m_Cert->Parse(CertFile.data(), CertFile.size());
		if (res == 0)
		{
			m_CertPrivKey.reset(new cCryptoKey);
			int res2 = m_CertPrivKey->ParsePrivate(KeyFile.data(), KeyFile.size(), "");
			if (res2 != 0)
			{
				// Reading the private key failed, reset the cert:
				LOGWARNING("WebServer: Cannot read HTTPS certificate private key: -0x%x", -res2);
				m_Cert.reset();
			}
		}
		else
		{
			LOGWARNING("WebServer: Cannot read HTTPS certificate: -0x%x", -res);
		}
	}

	// Notify the admin about the HTTPS / HTTP status
	if (m_Cert.get() == NULL)
	{
		LOGWARNING("WebServer: The server is running in unsecure HTTP mode.");
	}
	else
	{
		LOGINFO("WebServer: The server is running in secure HTTPS mode.");
	}
	
	// Open up requested ports:
	bool HasAnyPort;
	m_ListenThreadIPv4.SetReuseAddr(true);
	m_ListenThreadIPv6.SetReuseAddr(true);
	HasAnyPort = m_ListenThreadIPv4.Initialize(a_PortsIPv4);
	HasAnyPort = m_ListenThreadIPv6.Initialize(a_PortsIPv6) || HasAnyPort;
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
	if (!m_ListenThreadIPv6.Start())
	{
		m_ListenThreadIPv4.Stop();
		return false;
	}
	return true;
}





void cHTTPServer::Stop(void)
{
	m_ListenThreadIPv4.Stop();
	m_ListenThreadIPv6.Stop();
	
	// Drop all current connections:
	cCSLock Lock(m_CSConnections);
	while (!m_Connections.empty())
	{
		m_Connections.front()->Terminate();
	}  // for itr - m_Connections[]
}





void cHTTPServer::OnConnectionAccepted(cSocket & a_Socket)
{
	cHTTPConnection * Connection;
	if (m_Cert.get() != NULL)
	{
		Connection = new cSslHTTPConnection(*this, m_Cert, m_CertPrivKey);
	}
	else
	{
		Connection = new cHTTPConnection(*this);
	}
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





void cHTTPServer::RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, size_t a_Size)
{
	m_Callbacks->OnRequestBody(a_Connection, a_Request, a_Data, a_Size);
}





void cHTTPServer::RequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	m_Callbacks->OnRequestFinished(a_Connection, a_Request);
	a_Connection.AwaitNextRequest();
}




