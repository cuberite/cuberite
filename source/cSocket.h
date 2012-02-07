#pragma once





class cPacket;





class cSocket
{
public:
#ifdef _WIN32
	typedef SOCKET xSocket;
#else
	typedef int xSocket;
	static const int INVALID_SOCKET = -1;
#endif

	cSocket(void) : m_Socket(INVALID_SOCKET) {}
	cSocket(xSocket a_Socket);
	~cSocket();

	bool IsValid(void) const;
	void CloseSocket();

	operator const xSocket() const;
	xSocket GetSocket() const;
	void SetSocket( xSocket a_Socket );

	int SetReuseAddress();
	static int WSAStartup();

	static AString GetErrorString( int a_ErrNo );
	static int     GetLastError();
	static AString GetLastErrorString(void)
	{
		return GetErrorString(GetLastError());
	}
	
	static cSocket CreateSocket();

	inline static bool IsSocketError( int a_ReturnedValue )
	{
#ifdef _WIN32
		return (a_ReturnedValue == SOCKET_ERROR || a_ReturnedValue == 0);
#else
		return (a_ReturnedValue <= 0);
#endif
	}

	struct SockAddr_In
	{
		short Family;
		unsigned short Port;
		unsigned long Address;
	};

	static const short ADDRESS_FAMILY_INTERNET = 2;
	static const unsigned long INTERNET_ADDRESS_ANY = 0;
	static unsigned long INTERNET_ADDRESS_LOCALHOST(void);  // 127.0.0.1 represented in network byteorder; must be a function due to GCC :(

	int Bind( SockAddr_In& a_Address );
	int Listen( int a_Backlog );
	cSocket Accept();
	int Connect(SockAddr_In & a_Address);  // Returns 0 on success, !0 on failure
	int Receive( char* a_Buffer, unsigned int a_Length, unsigned int a_Flags );
	int Send   (const char * a_Buffer, unsigned int a_Length);
	int Send   (const cPacket * a_Packet);  // Sends the packet, doesn't handle partial sends
	int Send   (const cPacket & a_Packet);  // Sends the packet, doesn't handle partial sends
	
	unsigned short GetPort(void) const;  // Returns 0 on failure

	const AString & GetIPString(void) const { return m_IPString; }

private:
	xSocket m_Socket;
	AString m_IPString;
};