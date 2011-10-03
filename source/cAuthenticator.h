#pragma once

class cBlockingTCPLink;
class cAuthenticator
{
public:
	cAuthenticator();
	~cAuthenticator();

	bool Authenticate( const char* a_PlayerName, const char* a_ServerID );
private:
	bool ParseReceived( const char* a_Data, cBlockingTCPLink* a_TCPLink );
};