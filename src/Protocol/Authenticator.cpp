
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "OSSupport/BlockingTCPLink.h"
#include "Root.h"
#include "Server.h"

#include "inifile/iniFile.h"
#include "json/json.h"

#include "polarssl/config.h"
#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/error.h"
#include "polarssl/certs.h"

#include <sstream>
#include <iomanip>





#define DEFAULT_AUTH_SERVER "sessionserver.mojang.com"
#define DEFAULT_AUTH_ADDRESS "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%"
#define MAX_REDIRECTS 10





cAuthenticator::cAuthenticator(void) :
	super("cAuthenticator"),
	m_Server(DEFAULT_AUTH_SERVER),
	m_Address(DEFAULT_AUTH_ADDRESS),
	m_ShouldAuthenticate(true)
{
}





cAuthenticator::~cAuthenticator()
{
	Stop();
}





void cAuthenticator::ReadINI(cIniFile & IniFile)
{
	m_Server  = IniFile.GetValueSet("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address = IniFile.GetValueSet("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = IniFile.GetValueSetB("Authentication", "Authenticate", true);
}





void cAuthenticator::Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash)
{
	if (!m_ShouldAuthenticate)
	{
		cRoot::Get()->AuthenticateUser(a_ClientID, a_UserName, Printf("%d", a_ClientID));
		return;
	}
	
	cCSLock LOCK(m_CS);
	m_Queue.push_back(cUser(a_ClientID, a_UserName, a_ServerHash));
	m_QueueNonempty.Set();
}





void cAuthenticator::Start(cIniFile & IniFile)
{
	ReadINI(IniFile);
	m_ShouldTerminate = false;
	super::Start();
}





void cAuthenticator::Stop(void)
{
	m_ShouldTerminate = true;
	m_QueueNonempty.Set();
	Wait();
}





void cAuthenticator::Execute(void)
{
	for (;;)
	{
		cCSLock Lock(m_CS);
		while (!m_ShouldTerminate && (m_Queue.size() == 0))
		{
			cCSUnlock Unlock(Lock);
			m_QueueNonempty.Wait();
		}
		if (m_ShouldTerminate)
		{
			return;
		}
		ASSERT(!m_Queue.empty());
		
		int ClientID = m_Queue.front().m_ClientID;
		AString UserName = m_Queue.front().m_Name;
		AString ServerID = m_Queue.front().m_ServerID;
		m_Queue.pop_front();
		Lock.Unlock();

		AString NewUserName = UserName;
		AString UUID;
		if (AuthWithYggdrasil(NewUserName, ServerID, UUID))
		{
			cRoot::Get()->AuthenticateUser(ClientID, NewUserName, UUID);
			LOGINFO("User %s authenticated with UUID %s", NewUserName.c_str(), UUID.c_str());
		}
		else
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, AString & a_UUID)
{
	AString REQUEST;
	int ret, len, server_fd = -1;
	unsigned char buf[1024];
	const char *pers = "cAuthenticator";

	entropy_context entropy;
	ctr_drbg_context ctr_drbg;
	ssl_context ssl;
	x509_crt cacert;

	/* Initialize the RNG and the session data */
	memset( &ssl, 0, sizeof( ssl_context ) );
	x509_crt_init( &cacert );

	entropy_init( &entropy );
	if ( ( ret = ctr_drbg_init( &ctr_drbg, entropy_func, &entropy, (const unsigned char *) pers, strlen( pers ) ) ) != 0 )
	{
		LOGERROR("cAuthenticator: ctr_drbg_init returned %d", ret );
		return false;
	}

	/* Initialize certificates */

#if defined(POLARSSL_CERTS_C)
	ret = x509_crt_parse( &cacert, (const unsigned char *) test_ca_list, strlen( test_ca_list ) );
#else
	ret = 1;
	LOGWARNING("cAuthenticator: POLARSSL_CERTS_C is not defined.");
#endif

	if( ret < 0 )
	{
		LOGERROR("cAuthenticator: x509_crt_parse returned -0x%x", -ret);
		return false;
	}

	/* Connect */
	if( ( ret = net_connect( &server_fd, m_Server.c_str(), 443 ) ) != 0 )
	{
		LOGERROR("cAuthenticator: Can't connect to %s: %d", m_Server.c_str(), ret );
		return false;
	}

	/* Setup */
	if( ( ret = ssl_init( &ssl ) ) != 0 )
	{
		LOGERROR("cAuthenticator: ssl_init returned %d", ret);
		return false;
	}
	ssl_set_endpoint( &ssl, SSL_IS_CLIENT );
	ssl_set_authmode( &ssl, SSL_VERIFY_OPTIONAL );
	ssl_set_ca_chain( &ssl, &cacert, NULL, "PolarSSL Server 1" );
	ssl_set_rng( &ssl, ctr_drbg_random, &ctr_drbg );
	ssl_set_bio( &ssl, net_recv, &server_fd, net_send, &server_fd );
	
	/* Handshake */
	while( ( ret = ssl_handshake( &ssl ) ) != 0 )
	{
		if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
		{
			LOGERROR("cAuthenticator: ssl_handshake returned -0x%x", -ret);
			return false;
		}
	}

	/* Write the GET request */
	AString ActualAddress = m_Address;
	ReplaceString(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceString(ActualAddress, "%SERVERID%", a_ServerId);
	
	REQUEST += "GET " + ActualAddress + " HTTP/1.1\r\n";
	REQUEST += "Host: " + m_Server + "\r\n";
	REQUEST += "User-Agent: MCServer\r\n";
	REQUEST += "Connection: close\r\n";
	REQUEST += "\r\n";
	
	len = sprintf( (char *) buf, REQUEST.c_str() );

	while( ( ret = ssl_write( &ssl, buf, len ) ) <= 0 )
	{
		if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
		{
			LOGERROR("cAuthenticator: ssl_write returned %d", ret);
			return false;
		}
	}

    /* Read the HTTP response */
	std::string Builder;
	do
	{
		len = sizeof( buf ) - 1;
		memset( buf, 0, sizeof( buf ) );
		ret = ssl_read( &ssl, buf, len );
		buf[ret] = '\0';

		if( ret == POLARSSL_ERR_NET_WANT_READ || ret == POLARSSL_ERR_NET_WANT_WRITE )
			continue;
		if( ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY )
			break;
		if( ret < 0 )
		{
			LOGERROR("cAuthenticator: ssl_read returned %d", ret);
			break;
		}
		if( ret == 0 )
		{
			LOGERROR("cAuthenticator: EOF");
			break;
		}
		
		std::string str;
		str.append(reinterpret_cast<const char*>(buf));
		Builder += str;
	}
	while( 1 );
	
	ssl_close_notify( &ssl );
	x509_crt_free( &cacert );
	net_close( server_fd );
	ssl_free( &ssl );
	entropy_free( &entropy );
	memset( &ssl, 0, sizeof( ssl ) );
	
	std::string prefix("HTTP/1.1 200 OK");
	if (Builder.compare(0, prefix.size(), prefix))
		return false;
	
	std::stringstream ResponseBuilder;
	bool NewLine = false;
	bool IsNewLine = false;
	for (std::string::const_iterator i = Builder.begin(); i <= Builder.end(); ++i)
	{
		if (NewLine)
		{
			ResponseBuilder << *i;
		}
		else if (!NewLine && *i == '\n')
		{
			if (IsNewLine)
			{
				NewLine = true;
			}
			else
			{
				IsNewLine = true;
			}
		}
		else if (*i != '\r')
		{
			IsNewLine = false;
		}
	}
	
	AString RESPONSE = ResponseBuilder.str();
	
	if (RESPONSE.empty())
		return false;
	
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(RESPONSE, root, false))
	{
		LOGWARNING("cAuthenticator: Cannot parse Received Data to json!");
		return false;
	}
	
	a_UserName = root.get("name", "Unknown").asString();
	a_UUID = root.get("id", "").asString();
	return true;
}





