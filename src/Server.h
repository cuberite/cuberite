
// cServer.h

// Interfaces to the cServer object representing the network server





#pragma once

#include "RCONServer.h"
#include "OSSupport/IsThread.h"
#include "OSSupport/Network.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#include "PolarSSL++/RsaPrivateKey.h"

#ifdef _MSC_VER
	#pragma warning(pop)
#endif





// fwd:
class cPlayer;
class cClientHandle;
typedef SharedPtr<cClientHandle> cClientHandlePtr;
typedef std::list<cClientHandlePtr> cClientHandlePtrs;
typedef std::list<cClientHandle *> cClientHandles;
class cCommandOutputCallback;
class cSettingsRepositoryInterface;


namespace Json
{
	class Value;
}





// tolua_begin
class cServer
{
public:
	// tolua_end

	virtual ~cServer() {}
	bool InitServer(cSettingsRepositoryInterface & a_Settings, bool a_ShouldAuth);

	// tolua_begin

	const AString & GetDescription(void) const {return m_Description; }

	// Player counts:
	int  GetMaxPlayers(void) const { return m_MaxPlayers; }
	int  GetNumPlayers(void) const;
	void SetMaxPlayers(int a_MaxPlayers) { m_MaxPlayers = a_MaxPlayers; }

	/** Check if the player is queued to be transferred to a World.
	Returns true is Player is found in queue. */
	bool IsPlayerInQueue(AString a_Username);

	/** Can login more than once with same username.
	Returns false if it is not allowed, true otherwise. */
	bool DoesAllowMultiLogin(void) { return m_bAllowMultiLogin; }

	// Hardcore mode or not:
	bool IsHardcore(void) const { return m_bIsHardcore; }

	// tolua_end

	bool Start(void);

	bool Command(cClientHandle & a_Client, AString & a_Cmd);

	/** Executes the console command, sends output through the specified callback */
	void ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);

	/** Lists all available console commands and their helpstrings */
	void PrintHelp(const AStringVector & a_Split, cCommandOutputCallback & a_Output);

	/** Binds the built-in console commands with the plugin manager */
	static void BindBuiltInConsoleCommands(void);

	void Shutdown(void);

	void KickUser(int a_ClientID, const AString & a_Reason);

	/** Authenticates the specified user, called by cAuthenticator */
	void AuthenticateUser(int a_ClientID, const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties);

	const AString & GetServerID(void) const { return m_ServerID; }  // tolua_export

	/** Called by cClientHandle's destructor; stop m_SocketThreads from calling back into a_Client */
	void ClientDestroying(const cClientHandle * a_Client);

	/** Don't tick a_Client anymore, it will be ticked from its cPlayer instead */
	void ClientMovedToWorld(const cClientHandle * a_Client);

	/** Notifies the server that a player was created; the server uses this to adjust the number of players */
	void PlayerCreated(const cPlayer * a_Player);

	/** Notifies the server that a player is being destroyed; the server uses this to adjust the number of players */
	void PlayerDestroying(const cPlayer * a_Player);

	/** Returns base64 encoded favicon data (obtained from favicon.png) */
	const AString & GetFaviconData(void) const { return m_FaviconData; }

	cRsaPrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	const AString & GetPublicKeyDER(void) const { return m_PublicKeyDER; }

	/** Returns true if authentication has been turned on in server settings. */
	bool ShouldAuthenticate(void) const { return m_ShouldAuthenticate; }  // tolua_export

	/** Returns true if offline UUIDs should be used to load data for players whose normal UUIDs cannot be found.
	Loaded from the settings.ini [PlayerData].LoadOfflinePlayerData setting. */
	bool ShouldLoadOfflinePlayerData(void) const { return m_ShouldLoadOfflinePlayerData; }

	/** Returns true if old-style playernames should be used to load data for players whose regular datafiles cannot be found.
	This allows a seamless transition from name-based to UUID-based player storage.
	Loaded from the settings.ini [PlayerData].LoadNamedPlayerData setting. */
	bool ShouldLoadNamedPlayerData(void) const { return m_ShouldLoadNamedPlayerData; }

	/** Returns true if BungeeCord logins (that specify the player's UUID) are allowed.
	Read from settings, admins should set this to true only when they chain to BungeeCord,
	it makes the server vulnerable to identity theft through direct connections. */
	bool ShouldAllowBungeeCord(void) const { return m_ShouldAllowBungeeCord; }

private:

	friend class cRoot;  // so cRoot can create and destroy cServer
	friend class cServerListenCallbacks;  // Accessing OnConnectionAccepted()

	/** The server tick thread takes care of the players who aren't yet spawned in a world */
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


	/** The network sockets listening for client connections. */
	cServerHandlePtrs m_ServerHandles;

	/** Protects m_Clients and m_ClientsToRemove against multithreaded access. */
	cCriticalSection m_CSClients;

	/** Clients that are connected to the server and not yet assigned to a cWorld. */
	cClientHandlePtrs m_Clients;

	/** Clients that have just been moved into a world and are to be removed from m_Clients in the next Tick(). */
	cClientHandles m_ClientsToRemove;

	/** Protects m_PlayerCount against multithreaded access. */
	mutable cCriticalSection m_CSPlayerCount;

	/** Number of players currently playing in the server. */
	int m_PlayerCount;

	/** Protects m_PlayerCountDiff against multithreaded access. */
	cCriticalSection m_CSPlayerCountDiff;

	/** Adjustment to m_PlayerCount to be applied in the Tick thread. */
	int m_PlayerCountDiff;

	int m_ClientViewDistance;  // The default view distance for clients; settable in Settings.ini

	bool m_bIsConnected;  // true - connected false - not connected

	std::atomic<bool> m_bRestarting;

	/** The private key used for the assymetric encryption start in the protocols */
	cRsaPrivateKey m_PrivateKey;

	/** Public key for m_PrivateKey, ASN1-DER-encoded */
	AString m_PublicKeyDER;

	cRCONServer m_RCONServer;

	AString m_Description;
	AString m_FaviconData;
	int m_MaxPlayers;
	bool m_bIsHardcore;

	/** True - allow same username to login more than once False - only once */
	bool m_bAllowMultiLogin;

	cTickThread m_TickThread;
	cEvent m_RestartEvent;

	/** The server ID used for client authentication */
	AString m_ServerID;

	/** If true, players will be online-authenticated agains Mojang servers.
	This setting is the same as the "online-mode" setting in Vanilla. */
	bool m_ShouldAuthenticate;

	/** True if offline UUIDs should be used to load data for players whose normal UUIDs cannot be found.
	This allows transitions from an offline (no-auth) server to an online one.
	Loaded from the settings.ini [PlayerData].LoadOfflinePlayerData setting. */
	bool m_ShouldLoadOfflinePlayerData;

	/** True if old-style playernames should be used to load data for players whose regular datafiles cannot be found.
	This allows a seamless transition from name-based to UUID-based player storage.
	Loaded from the settings.ini [PlayerData].LoadNamedPlayerData setting. */
	bool m_ShouldLoadNamedPlayerData;

	/** True if BungeeCord handshake packets (with player UUID) should be accepted. */
	bool m_ShouldAllowBungeeCord;

	/** The list of ports on which the server should listen for connections.
	Initialized in InitServer(), used in Start(). */
	AStringVector m_Ports;


	cServer(void);

	/** Loads, or generates, if missing, RSA keys for protocol encryption */
	void PrepareKeys(void);

	/** Creates a new cClientHandle instance and adds it to the list of clients.
	Returns the cClientHandle reinterpreted as cTCPLink callbacks. */
	cTCPLink::cCallbacksPtr OnConnectionAccepted(const AString & a_RemoteIPAddress);

	bool Tick(float a_Dt);

	/** Ticks the clients in m_Clients, manages the list in respect to removing clients */
	void TickClients(float a_Dt);
};  // tolua_export




