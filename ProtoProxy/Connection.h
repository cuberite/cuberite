
// Connection.h

// Interfaces to the cConnection class representing a single pair of connected sockets





#pragma once

#include <time.h>





class cServer;





class cConnection
{
	cCriticalSection m_CSLog;
	FILE * m_LogFile;
	
	cServer & m_Server;
	SOCKET m_ClientSocket;
	SOCKET m_ServerSocket;
	
	clock_t m_BeginTick;  // Tick when the relative time was first retrieved (used for GetRelativeTime())
	
public:
	cConnection(SOCKET a_ClientSocket, cServer & a_Server);
	~cConnection();
	
	void Run(void);
	
	void Log(const char * a_Format, ...);
	void DataLog(const void * a_Data, int a_Size, const char * a_Format, ...);
	
protected:
	bool ConnectToServer(void);
	
	/// Relays data from server to client; returns false if connection aborted
	bool RelayFromServer(void);
	
	/// Relays data from client to server; returns false if connection aborted
	bool RelayFromClient(void);
	
	/// Returns the time relative to the first call of this function, in the fractional seconds elapsed
	double GetRelativeTime(void);
} ;




