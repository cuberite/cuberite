
// cServer.h

// Interfaces to the cServer object representing the network server





#pragma once

#include "OSSupport/SocketThreads.h"
#include "OSSupport/ListenThread.h"
#include "CryptoPP/rsa.h"
#include "CryptoPP/randpool.h"
#include "RCONServer.h"





// fwd:
class cPlayer;
class cClientHandle;
class cIniFile;
class cCommandOutputCallback ;

typedef std::list<cClientHandle *> cClientHandleList;





class cServer										// tolua_export
	: public cListenThread::cCallback
{													// tolua_export
public:												// tolua_export
	bool InitServer(cIniFile & a_SettingsIni);

	bool IsConnected(void) const { return m_bIsConnected;} // returns connection status
	
	void BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude = NULL);  // tolua_export

	bool Tick(float a_Dt);

	bool Start(void);

	bool Command(cClientHandle & a_Client, AString & a_Cmd);
	
	/// Executes the console command, sends output through the specified callback
	void ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);

	/// Binds the built-in console commands with the plugin manager
	static void BindBuiltInConsoleCommands(void);
	
	void Shutdown();

	void SendMessage(const AString & a_Message, cPlayer * a_Player = NULL, bool a_bExclude = false );  // tolua_export
	
	void KickUser(int a_ClientID, const AString & a_Reason);
	void AuthenticateUser(int a_ClientID);  // Called by cAuthenticator to auth the specified user

	const AString & GetServerID(void) const; // tolua_export
	
	void ClientDestroying(const cClientHandle * a_Client);  // Called by cClientHandle::Destroy(); stop m_SocketThreads from calling back into a_Client
	
	void NotifyClientWrite(const cClientHandle * a_Client);  // Notifies m_SocketThreads that client has something to be written
	
	void WriteToClient(const cClientHandle * a_Client, const AString & a_Data);  // Queues outgoing data for the client through m_SocketThreads
	
	void QueueClientClose(const cClientHandle * a_Client);  // Queues the clienthandle to close when all its outgoing data is sent
	
	void RemoveClient(const cClientHandle * a_Client);  // Removes the clienthandle from m_SocketThreads
	
	CryptoPP::RSA::PrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	CryptoPP::RSA::PublicKey  & GetPublicKey (void) { return m_PublicKey; }
	
private:

	friend class cRoot; // so cRoot can create and destroy cServer
	
	/// When NotifyClientWrite() is called, it is queued for this thread to process (to avoid deadlocks between cSocketThreads, cClientHandle and cChunkMap)
	class cNotifyWriteThread :
		public cIsThread
	{
		typedef cIsThread super;
		
		cEvent    m_Event;  // Set when m_Clients gets appended
		cServer * m_Server;

		cCriticalSection  m_CS;
		cClientHandleList m_Clients;
		
		virtual void Execute(void);
		
	public:	
	
		cNotifyWriteThread(void);
		~cNotifyWriteThread();
	
		bool Start(cServer * a_Server);
		
		void NotifyClientWrite(const cClientHandle * a_Client);
	} ;
	
	struct sServerState;
	sServerState * m_pState;
	
	cNotifyWriteThread m_NotifyWriteThread;
	
	cListenThread m_ListenThreadIPv4;
	cListenThread m_ListenThreadIPv6;
	
	cCriticalSection  m_CSClients;  // Locks client list
	cClientHandleList m_Clients;    // Clients that are connected to the server
	
	cSocketThreads m_SocketThreads;
	
	int m_ClientViewDistance;  // The default view distance for clients; settable in Settings.ini

	// Time since server was started
	float m_Millisecondsf;
	unsigned int m_Milliseconds;

	bool m_bIsConnected; // true - connected false - not connected

	bool m_bRestarting;
	
	CryptoPP::RSA::PrivateKey m_PrivateKey;
	CryptoPP::RSA::PublicKey  m_PublicKey;
	
	cRCONServer m_RCONServer;
	

	cServer(void);
	~cServer();

	/// Loads, or generates, if missing, RSA keys for protocol encryption
	void PrepareKeys(void);
	
	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
}; // tolua_export




