
#pragma once





// Windows and MacOSX don't have the MSG_NOSIGNAL flag
#if ( \
	defined(_WIN32) || \
	(defined(__APPLE__) && defined(__MACH__)) \
)
	#define MSG_NOSIGNAL (0)
#endif


#include "Errors.h"


class cSocketSet;


class cSocket
{
public:
	enum eFamily
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
		IPDual,
		INVALID_PROTOCOL
	} ;
	
	enum eFlags
	{
	
		#ifdef _WIN32
		ErrWouldBlock = WSAEWOULDBLOCK,
		#else
		ErrWouldBlock = EWOULDBLOCK,
		#endif
		
	} ;
	
#ifdef _WIN32
	typedef SOCKET xSocket;
#else
	typedef int xSocket;
	static const int INVALID_SOCKET = -1;
#endif

	cSocket(void) : m_Socket(INVALID_SOCKET), m_family(INVALID_PROTOCOL) {}
	~cSocket();

	bool IsValid(void) const { return IsValidSocket(m_Socket); }
	void CloseSocket(void);
	
	/** Notifies the socket that we don't expect any more reads nor writes on it.
	Most TCPIP implementations use this to send the FIN flag in a packet */
	void ShutdownReadWrite(void);
	
	bool operator == (const cSocket & a_Other) const {return m_Socket == a_Other.m_Socket; }
	
	bool operator < (const cSocket & a_Other) const {return m_Socket < a_Other.m_Socket; }
	
	bool operator > (const cSocket & a_Other) const {return m_Socket > a_Other.m_Socket; }

	/// Sets the address-reuse socket flag; returns true on success
	bool SetReuseAddress(void);
	
	/// Initializes the network stack. Returns 0 on success, or another number as an error code.
	static int WSAStartup(void);

	static int     GetLastError();
	static AString GetLastErrorString(void)
	{
		return GetOSErrorString(GetLastError());
	}
	
	/// Creates a new socket of the specified address family
	static cSocket CreateSocket(eFamily a_Family);

	inline static bool IsSocketError(int a_ReturnedValue)
	{
		#ifdef _WIN32
			return (a_ReturnedValue == SOCKET_ERROR || a_ReturnedValue == 0);
		#else
			return (a_ReturnedValue <= 0);
		#endif
	}
	
	static bool IsValidSocket(xSocket a_Socket);

	static const unsigned short ANY_PORT = 0;  // When given to Bind() functions, they will find a free port
	static const int DEFAULT_BACKLOG = 10;

	/// Binds to the specified port on "any" interface (0.0.0.0 for IPv4, ::/128 for IPv6). Returns true if successful.
	bool BindToAny(unsigned short a_Port);
	
	/// Binds to the specified port on localhost interface (127.0.0.1) through IPv4. Returns true if successful.
	bool BindToLocalhostIPv4(unsigned short a_Port);
	
	/// Sets the socket to listen for incoming connections. Returns true if successful.
	bool Listen(int a_Backlog = DEFAULT_BACKLOG);
	
	/// Accepts an incoming connection. Blocks if none available.
	cSocket Accept(void);
	
	/// Connects to a localhost socket on the specified port using IPv4; returns true if successful.
	bool ConnectToLocalhostIPv4(unsigned short a_Port);
	
	/// Connects to the specified host or string IP address and port, using IPv4. Returns true if successful.
	bool ConnectIPv4(const AString & a_HostNameOrAddr, unsigned short a_Port);
	
	int Receive(char * a_Buffer, unsigned int a_Length, unsigned int a_Flags);
	int Send   (const char * a_Buffer, unsigned int a_Length);
	
	unsigned short GetPort(void) const;  // Returns 0 on failure

	const AString & GetIPString(void) const { return m_IPString; }
	
	/** Sets the socket into non-blocking mode */
	void SetNonBlocking(void);
	
	static bool SupportsFamily(eFamily a_Family)
	{
		switch(a_Family)
		{
			case IPv4:
			case IPv6:
			{
				return true;
			}
			case IPDual:
			{
			#ifdef _WIN32
				return IsWindowsVista();
			#else
				return true;
			#endif
			}
			case INVALID_PROTOCOL:
			{
				return false;
			}
		}
	}
	
private:
	friend class cSocketSet;
	xSocket m_Socket;
	AString m_IPString;
	eFamily m_family;
	
	cSocket(xSocket a_Socket, eFamily a_family);
	
	#ifdef _WIN32
	static bool IsVistaOrLater();
	#endif
};
