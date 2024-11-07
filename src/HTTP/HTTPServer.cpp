
// HTTPServer.cpp

// Implements the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing

#include "Globals.h"
#include "HTTPServer.h"
#include "HTTPMessageParser.h"
#include "HTTPServerConnection.h"
#include "HTTPFormParser.h"
#include "SslHTTPServerConnection.h"
#include "../mbedTLS++/SslConfig.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





////////////////////////////////////////////////////////////////////////////////
// cHTTPServerListenCallbacks:

class cHTTPServerListenCallbacks:
	public cNetwork::cListenCallbacks
{
public:
	cHTTPServerListenCallbacks(cHTTPServer & a_HTTPServer, UInt16 a_Port):
		m_HTTPServer(a_HTTPServer),
		m_Port(a_Port)
	{
	}

protected:
	/** The HTTP server instance that we're attached to. */
	cHTTPServer & m_HTTPServer;

	/** The port for which this instance is responsible. */
	UInt16 m_Port;

	// cNetwork::cListenCallbacks overrides:
	virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort) override
	{
		return m_HTTPServer.OnIncomingConnection(a_RemoteIPAddress, a_RemotePort);
	}
	virtual void OnAccepted(cTCPLink & a_Link) override {}
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		LOGWARNING("HTTP server error on port %d: %d (%s)", m_Port, a_ErrorCode, a_ErrorMsg.c_str());
	}
};





////////////////////////////////////////////////////////////////////////////////
// cHTTPServer:

cHTTPServer::cHTTPServer(void) :
	m_Callbacks(nullptr)
{
}





cHTTPServer::~cHTTPServer()
{
	Stop();
}





bool cHTTPServer::Initialize(void)
{
	// Read the HTTPS cert + key:
	AString CertFile = cFile::ReadWholeFile("webadmin/httpscert.crt");
	AString KeyFile  = cFile::ReadWholeFile("webadmin/httpskey.pem");
	if (!CertFile.empty() && !KeyFile.empty())
	{
		auto Cert = std::make_shared<cX509Cert>();
		int res = Cert->Parse(CertFile.data(), CertFile.size());
		if (res == 0)
		{
			auto CertPrivKey = std::make_shared<cCryptoKey>();
			res = CertPrivKey->ParsePrivate(KeyFile.data(), KeyFile.size(), "");
			if (res == 0)
			{
				// Modifyable locally but otherwise must be const
				auto Config = cSslConfig::MakeDefaultConfig(false);
				Config->SetOwnCert(Cert, CertPrivKey);
				m_SslConfig = std::move(Config);
			}
			else
			{
				// Reading the private key failed, reset the cert:
				LOGWARNING("WebServer: Cannot read HTTPS certificate private key: -0x%x", -res);
			}
		}
		else
		{
			LOGWARNING("WebServer: Cannot read HTTPS certificate: -0x%x", -res);
		}
	}

	// Notify the admin about the HTTPS / HTTP status
	if (m_SslConfig == nullptr)
	{
		LOGWARNING("WebServer: The server will run in unsecured HTTP mode.");
		LOGINFO("Put a valid HTTPS certificate in file 'webadmin/httpscert.crt' and its corresponding private key to 'webadmin/httpskey.pem' (without any password) to enable HTTPS support");
	}
	else
	{
		LOGINFO("WebServer: The server will run in secure HTTPS mode.");
	}
	return true;
}





bool cHTTPServer::Start(cCallbacks & a_Callbacks, const AStringVector & a_Ports)
{
	m_Callbacks = &a_Callbacks;

	// Open up requested ports:
	AStringVector ports;
	for (const auto & port : a_Ports)
	{
		UInt16 PortNum;
		if (!StringToInteger(port, PortNum))
		{
			LOGWARNING("WebServer: Invalid port value: \"%s\". Ignoring.", port.c_str());
			continue;
		}
		auto Handle = cNetwork::Listen(PortNum, std::make_shared<cHTTPServerListenCallbacks>(*this, PortNum));
		if (Handle->IsListening())
		{
			m_ServerHandles.push_back(Handle);
			ports.push_back(port);
		}
	}  // for port - a_Ports[]

	// Inform the admin about the ports opened:
	AString reportPorts;
	for (const auto & port: ports)
	{
		if (!reportPorts.empty())
		{
			reportPorts.append(", ");
		}
		reportPorts.append(port);
	}
	LOGINFO("WebAdmin is running on port(s) %s", reportPorts.c_str());

	// Report success if at least one port opened successfully:
	return !m_ServerHandles.empty();
}





void cHTTPServer::Stop(void)
{
	for (const auto & handle : m_ServerHandles)
	{
		handle->Close();
	}
	m_ServerHandles.clear();
}





cTCPLink::cCallbacksPtr cHTTPServer::OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort)
{
	UNUSED(a_RemoteIPAddress);
	UNUSED(a_RemotePort);

	if (m_SslConfig != nullptr)
	{
		return std::make_shared<cSslHTTPServerConnection>(*this, m_SslConfig);
	}
	else
	{
		return std::make_shared<cHTTPServerConnection>(*this);
	}
}





void cHTTPServer::NewRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	m_Callbacks->OnRequestBegun(a_Connection, a_Request);
}





void cHTTPServer::RequestBody(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request, const void * a_Data, size_t a_Size)
{
	m_Callbacks->OnRequestBody(a_Connection, a_Request, static_cast<const char *>(a_Data), a_Size);
}





void cHTTPServer::RequestFinished(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	m_Callbacks->OnRequestFinished(a_Connection, a_Request);
}




