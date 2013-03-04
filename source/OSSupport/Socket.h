
#pragma once





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

	bool IsValid(void) const { return IsValidSocket(m_Socket); }
	void CloseSocket();

	operator xSocket() const;
	xSocket GetSocket() const;
	
	bool operator == (const cSocket & a_Other) {return m_Socket == a_Other.m_Socket; }
	
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
	
	static bool IsValidSocket(xSocket a_Socket);

	struct SockAddr_In
	{
		short Family;
		unsigned short Port;
		unsigned long Address;
	};

	static const short ADDRESS_FAMILY_INTERNET = 2;
	static const unsigned long INTERNET_ADDRESS_ANY = 0;
	static unsigned long INTERNET_ADDRESS_LOCALHOST(void);  // 127.0.0.1 represented in network byteorder; must be a function due to GCC :(
	static const unsigned short ANY_PORT = 0;  // When given to Bind() functions, they will find a free port
	static const int DEFAULT_BACKLOG = 10;

	/// Binds to the specified port on "any" interface (0.0.0.0)
	int BindToAny(unsigned short a_Port);
	
	/*
	// TODO:
	/// Binds to the specified port
	int BindToAny6(unsigned short a_Port);
	*/
	
	/// Binds to the specified port on localhost interface (127.0.0.1) through IPv4
	int BindToLocalhost(unsigned short a_Port);
	
	int Listen(int a_Backlog = DEFAULT_BACKLOG);
	cSocket Accept();
	
	int Connect(SockAddr_In & a_Address);  // Returns 0 on success, !0 on failure
	
	int Connect(const AString & a_HostNameOrAddr, unsigned short a_Port);  // Returns 0 on success, !0 on failure
	int Receive(char * a_Buffer, unsigned int a_Length, unsigned int a_Flags);
	int Send   (const char * a_Buffer, unsigned int a_Length);
	
	unsigned short GetPort(void) const;  // Returns 0 on failure

	const AString & GetIPString(void) const { return m_IPString; }

private:
	xSocket m_Socket;
	AString m_IPString;
};