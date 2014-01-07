
// cServer.h

// Interfaces to the cServer object representing the network server





#pragma once

#include "OSSupport/SocketThreads.h"
#include "OSSupport/ListenThread.h"

#include "RCONServer.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#include "cryptopp/rsa.h"
#include "cryptopp/randpool.h"

#ifdef _MSC_VER
	#pragma warning(pop)
#endif





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

	virtual ~cServer() {}
	bool InitServer(cIniFile & a_SettingsIni);

	// tolua_begin
	
	const AString & GetDescription(void) const {return m_Description; }

	// Player counts:
	int  GetMaxPlayers(void) const {return m_MaxPlayers; }
	int  GetNumPlayers(void);
	void SetMaxPlayers(int a_MaxPlayers) { m_MaxPlayers = a_MaxPlayers; }
	
	// Hardcore mode or not:
	bool IsHardcore(void) const {return m_bIsHardcore; }

	// tolua_end

	bool Start(void);

	bool Command(cClientHandle & a_Client, AString & a_Cmd);
	
	/// Executes the console command, sends output through the specified callback
	void ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);
	
	/// Lists all available console commands and their helpstrings
	void PrintHelp(const AStringVector & a_Split, cCommandOutputCallback & a_Output);

	/// Binds the built-in console commands with the plugin manager
	static void BindBuiltInConsoleCommands(void);
	
	void Shutdown(void);

	void KickUser(int a_ClientID, const AString & a_Reason);
	void AuthenticateUser(int a_ClientID);  // Called by cAuthenticator to auth the specified user

	const AString & GetServerID(void) const { return m_ServerID; }  // tolua_export
	
	void ClientDestroying(const cClientHandle * a_Client);  // Called by cClientHandle::Destroy(); stop m_SocketThreads from calling back into a_Client
	
	void NotifyClientWrite(const cClientHandle * a_Client);  // Notifies m_SocketThreads that client has something to be written
	
	void WriteToClient(const cClientHandle * a_Client, const AString & a_Data);  // Queues outgoing data for the client through m_SocketThreads
	
	void QueueClientClose(const cClientHandle * a_Client);  // Queues the clienthandle to close when all its outgoing data is sent
	
	void RemoveClient(const cClientHandle * a_Client);  // Removes the clienthandle from m_SocketThreads
	
	/// Don't tick a_Client anymore, it will be ticked from its cPlayer instead
	void ClientMovedToWorld(const cClientHandle * a_Client);
	
	/// Notifies the server that a player was created; the server uses this to adjust the number of players
	void PlayerCreated(const cPlayer * a_Player);
	
	/// Notifies the server that a player is being destroyed; the server uses this to adjust the number of players
	void PlayerDestroying(const cPlayer * a_Player);

	/* Returns base64 encoded favicon data (obtained from favicon.png) */
	const AString & GetFaviconData(void) const { return m_FaviconData; }
	
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
	
	/// The server tick thread takes care of the players who aren't yet spawned in a world
	class cTickThread :
		public cIsThread
	{
		typedef cIsThread super;
		
	public:
		cTickThread(cServer & a_Server);
		
	protected:
		cServer & m_Server;
		
		// cIsThread overrides:
		virtual void Execute(void) override;
	} ;
	
	
	cNotifyWriteThread m_NotifyWriteThread;
	
	cListenThread m_ListenThreadIPv4;
	cListenThread m_ListenThreadIPv6;
	
	cCriticalSection  m_CSClients;        ///< Locks client lists
	cClientHandleList m_Clients;          ///< Clients that are connected to the server and not yet assigned to a cWorld
	cClientHandleList m_ClientsToRemove;  ///< Clients that have just been moved into a world and are to be removed from m_Clients in the next Tick()
	
	cCriticalSection m_CSPlayerCount;      ///< Locks the m_PlayerCount
	int              m_PlayerCount;        ///< Number of players currently playing in the server
	cCriticalSection m_CSPlayerCountDiff;  ///< Locks the m_PlayerCountDiff
	int              m_PlayerCountDiff;    ///< Adjustment to m_PlayerCount to be applied in the Tick thread
	
	cSocketThreads m_SocketThreads;
	
	int m_ClientViewDistance;  // The default view distance for clients; settable in Settings.ini

	bool m_bIsConnected; // true - connected false - not connected

	bool m_bRestarting;
	
	CryptoPP::RSA::PrivateKey m_PrivateKey;
	CryptoPP::RSA::PublicKey  m_PublicKey;
	
	cRCONServer m_RCONServer;
	
	AString m_Description;
	AString m_FaviconData;
	int m_MaxPlayers;
	bool m_bIsHardcore;
	
	cTickThread m_TickThread;
	cEvent m_RestartEvent;
	
	/// The server ID used for client authentication
	AString m_ServerID;
	

	cServer(void);

	/// Loads, or generates, if missing, RSA keys for protocol encryption
	void PrepareKeys(void);
	
	bool Tick(float a_Dt);
	
	/// Ticks the clients in m_Clients, manages the list in respect to removing clients
	void TickClients(float a_Dt);

	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
}; // tolua_export




