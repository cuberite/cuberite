// ReDucTor is an awesome guy who helped me a lot

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Server.h"
#include "ClientHandle.h"
#include "Mobs/Monster.h"
#include "Root.h"
#include "World.h"
#include "ChunkDef.h"
#include "Bindings/PluginManager.h"
#include "ChatColor.h"
#include "Entities/Player.h"
#include "Inventory.h"
#include "Item.h"
#include "FurnaceRecipe.h"
#include "WebAdmin.h"
#include "Protocol/ProtocolRecognizer.h"
#include "CommandOutput.h"
#include "FastRandom.h"

#include "IniFile.h"
#include "Vector3.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern "C"
{
	#include "zlib/zlib.h"
}





typedef std::list< cClientHandle* > ClientList;





////////////////////////////////////////////////////////////////////////////////
// cServerListenCallbacks:

class cServerListenCallbacks:
	public cNetwork::cListenCallbacks
{
	cServer & m_Server;
	UInt16 m_Port;

	virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort) override
	{
		return m_Server.OnConnectionAccepted(a_RemoteIPAddress);
	}

	virtual void OnAccepted(cTCPLink & a_Link) override {}

	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		LOGWARNING("Cannot listen on port %d: %d (%s).", m_Port, a_ErrorCode, a_ErrorMsg.c_str());
	}

public:
	cServerListenCallbacks(cServer & a_Server, UInt16 a_Port):
		m_Server(a_Server),
		m_Port(a_Port)
	{
	}
};





////////////////////////////////////////////////////////////////////////////////
// cServer::cTickThread:

cServer::cTickThread::cTickThread(cServer & a_Server) :
	super("ServerTickThread"),
	m_Server(a_Server)
{
}





void cServer::cTickThread::Execute(void)
{
	auto LastTime = std::chrono::steady_clock::now();
	static const auto msPerTick = std::chrono::milliseconds(50);

	while (!m_ShouldTerminate)
	{
		auto NowTime = std::chrono::steady_clock::now();
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(NowTime - LastTime).count();
		m_ShouldTerminate = !m_Server.Tick(static_cast<float>(msec));
		auto TickTime = std::chrono::steady_clock::now() - NowTime;

		if (TickTime < msPerTick)
		{
			// Stretch tick time until it's at least msPerTick
			std::this_thread::sleep_for(msPerTick - TickTime);
		}

		LastTime = NowTime;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cServer:

cServer::cServer(void) :
	m_PlayerCount(0),
	m_ClientViewDistance(0),
	m_bIsConnected(false),
	m_bRestarting(false),
	m_RCONServer(*this),
	m_MaxPlayers(0),
	m_bIsHardcore(false),
	m_TickThread(*this),
	m_ShouldAuthenticate(false),
	m_ShouldLoadOfflinePlayerData(false),
	m_ShouldLoadNamedPlayerData(true)
{
	// Initialize the LuaStateTracker singleton before the app goes multithreaded:
	cLuaStateTracker::GetStats();
}





void cServer::ClientMovedToWorld(const cClientHandle * a_Client)
{
	cCSLock Lock(m_CSClients);
	m_ClientsToRemove.push_back(const_cast<cClientHandle *>(a_Client));
}





void cServer::PlayerCreated()
{
	m_PlayerCount++;
}





void cServer::PlayerDestroyed()
{
	m_PlayerCount--;
}





bool cServer::InitServer(cSettingsRepositoryInterface & a_Settings, bool a_ShouldAuth)
{
	m_Description = a_Settings.GetValueSet("Server", "Description", "Cuberite - in C++!");
	m_ShutdownMessage = a_Settings.GetValueSet("Server", "ShutdownMessage", "Server shutdown");
	m_MaxPlayers = static_cast<size_t>(a_Settings.GetValueSetI("Server", "MaxPlayers", 100));
	m_bIsHardcore = a_Settings.GetValueSetB("Server", "HardcoreEnabled", false);
	m_bAllowMultiLogin = a_Settings.GetValueSetB("Server", "AllowMultiLogin", false);

	m_FaviconData = Base64Encode(cFile::ReadWholeFile(FILE_IO_PREFIX + AString("favicon.png")));  // Will return empty string if file nonexistant; client doesn't mind

	if (m_bIsConnected)
	{
		LOGERROR("ERROR: Trying to initialize server while server is already running!");
		return false;
	}

	LOGINFO("Compatible clients: %s", MCS_CLIENT_VERSIONS);
	LOGINFO("Compatible protocol versions %s", MCS_PROTOCOL_VERSIONS);

	m_Ports = ReadUpgradeIniPorts(a_Settings, "Server", "Ports", "Port", "PortsIPv6", "25565");

	m_RCONServer.Initialize(a_Settings);

	m_bIsConnected = true;

	m_ServerID = "-";
	m_ShouldAuthenticate = a_ShouldAuth;
	if (m_ShouldAuthenticate)
	{
		auto & rand = GetRandomProvider();
		unsigned int r1 = rand.RandInt<unsigned int>(1000000000U, 0x7fffffffU);
		unsigned int r2 = rand.RandInt<unsigned int>(1000000000U, 0x7fffffffU);
		std::ostringstream sid;
		sid << std::hex << r1;
		sid << std::hex << r2;
		m_ServerID = sid.str();
		m_ServerID.resize(16, '0');
	}

	// Check if both BungeeCord and online mode are on, if so, warn the admin:
	m_ShouldAllowBungeeCord = a_Settings.GetValueSetB("Authentication", "AllowBungeeCord", false);
	if (m_ShouldAllowBungeeCord && m_ShouldAuthenticate)
	{
		LOGWARNING("WARNING: BungeeCord is allowed and server set to online mode. This is unsafe and will not work properly. Disable either authentication or BungeeCord in settings.ini.");
	}

	m_ShouldAllowMultiWorldTabCompletion = a_Settings.GetValueSetB("Server", "AllowMultiWorldTabCompletion", true);
	m_ShouldLimitPlayerBlockChanges = a_Settings.GetValueSetB("AntiCheat", "LimitPlayerBlockChanges", true);
	m_ShouldLoadOfflinePlayerData = a_Settings.GetValueSetB("PlayerData", "LoadOfflinePlayerData", false);
	m_ShouldLoadNamedPlayerData   = a_Settings.GetValueSetB("PlayerData", "LoadNamedPlayerData", true);

	m_ClientViewDistance = a_Settings.GetValueSetI("Server", "DefaultViewDistance", cClientHandle::DEFAULT_VIEW_DISTANCE);
	if (m_ClientViewDistance < cClientHandle::MIN_VIEW_DISTANCE)
	{
		m_ClientViewDistance = cClientHandle::MIN_VIEW_DISTANCE;
		LOGINFO("Setting default viewdistance to the minimum of %d", m_ClientViewDistance);
	}
	if (m_ClientViewDistance > cClientHandle::MAX_VIEW_DISTANCE)
	{
		m_ClientViewDistance = cClientHandle::MAX_VIEW_DISTANCE;
		LOGINFO("Setting default viewdistance to the maximum of %d", m_ClientViewDistance);
	}

	PrepareKeys();

	return true;
}





bool cServer::RegisterForgeMod(AString & a_ModName, AString & a_ModVersion, UInt32 a_ProtocolVersionNumber)
{
	auto & Mods = GetRegisteredForgeMods(a_ProtocolVersionNumber);

	return Mods.insert({a_ModName, a_ModVersion}).second;
}





void cServer::UnregisterForgeMod(AString &a_ModName, UInt32 a_ProtocolVersionNumber)
{
	auto & Mods = GetRegisteredForgeMods(a_ProtocolVersionNumber);

	auto it = Mods.find(a_ModName);
	if (it != Mods.end())
	{
		Mods.erase(it);
	}
}




AStringMap & cServer::GetRegisteredForgeMods(const UInt32 a_Protocol)
{
	auto it = m_ForgeModsByVersion.find(a_Protocol);

	if (it == m_ForgeModsByVersion.end())
	{
		AStringMap mods;
		m_ForgeModsByVersion.insert({a_Protocol, mods});
		return m_ForgeModsByVersion.find(a_Protocol)->second;
	}

	return it->second;
}




bool cServer::IsPlayerInQueue(AString a_Username)
{
	cCSLock Lock(m_CSClients);
	for (auto client : m_Clients)
	{
		if ((client->GetUsername()).compare(a_Username) == 0)
		{
			return true;
		}
	}
	return false;
}





void cServer::PrepareKeys(void)
{
	LOGD("Generating protocol encryption keypair...");
	VERIFY(m_PrivateKey.Generate(1024));
	m_PublicKeyDER = m_PrivateKey.GetPubKeyDER();
}





cTCPLink::cCallbacksPtr cServer::OnConnectionAccepted(const AString & a_RemoteIPAddress)
{
	LOGD("Client \"%s\" connected!", a_RemoteIPAddress.c_str());
	cClientHandlePtr NewHandle = std::make_shared<cClientHandle>(a_RemoteIPAddress, m_ClientViewDistance);
	NewHandle->SetSelf(NewHandle);
	cCSLock Lock(m_CSClients);
	m_Clients.push_back(NewHandle);
	return NewHandle;
}





bool cServer::Tick(float a_Dt)
{
	// Send the tick to the plugins, as well as let the plugin manager reload, if asked to (issue #102):
	cPluginManager::Get()->Tick(a_Dt);

	// Let the Root process all the queued commands:
	cRoot::Get()->TickCommands();

	// Tick all clients not yet assigned to a world:
	TickClients(a_Dt);

	if (!m_bRestarting)
	{
		return true;
	}
	else
	{
		m_bRestarting = false;
		m_RestartEvent.Set();
		return false;
	}
}





void cServer::TickClients(float a_Dt)
{
	cClientHandlePtrs RemoveClients;
	{
		cCSLock Lock(m_CSClients);

		// Remove clients that have moved to a world (the world will be ticking them from now on)
		for (auto itr = m_ClientsToRemove.begin(), end = m_ClientsToRemove.end(); itr != end; ++itr)
		{
			for (auto itrC = m_Clients.begin(), endC = m_Clients.end(); itrC != endC; ++itrC)
			{
				if (itrC->get() == *itr)
				{
					m_Clients.erase(itrC);
					break;
				}
			}
		}  // for itr - m_ClientsToRemove[]
		m_ClientsToRemove.clear();

		// Tick the remaining clients, take out those that have been destroyed into RemoveClients
		for (auto itr = m_Clients.begin(); itr != m_Clients.end();)
		{
			if ((*itr)->IsDestroyed())
			{
				// Delete the client later, when CS is not held, to avoid deadlock: https://forum.cuberite.org/thread-374.html
				RemoveClients.push_back(*itr);
				itr = m_Clients.erase(itr);
				continue;
			}
			(*itr)->ServerTick(a_Dt);
			++itr;
		}  // for itr - m_Clients[]
	}

	// Delete the clients that have been destroyed
	RemoveClients.clear();
}





bool cServer::Start(void)
{
	for (auto port: m_Ports)
	{
		UInt16 PortNum;
		if (!StringToInteger(port, PortNum))
		{
			LOGWARNING("Invalid port specified for server: \"%s\". Ignoring.", port.c_str());
			continue;
		}
		auto Handle = cNetwork::Listen(PortNum, std::make_shared<cServerListenCallbacks>(*this, PortNum));
		if (Handle->IsListening())
		{
			m_ServerHandles.push_back(Handle);
		}
	}  // for port - Ports[]
	if (m_ServerHandles.empty())
	{
		LOGERROR("Couldn't open any ports. Aborting the server");
		return false;
	}
	if (!m_TickThread.Start())
	{
		return false;
	}
	return true;
}





bool cServer::Command(cClientHandle & a_Client, AString & a_Cmd)
{
	return cRoot::Get()->GetPluginManager()->CallHookChat(*(a_Client.GetPlayer()), a_Cmd);
}





void cServer::ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output)
{
	AStringVector split = StringSplit(a_Cmd, " ");
	if (split.empty())
	{
		return;
	}

	// "stop" and "restart" are handled in cRoot::ExecuteConsoleCommand, our caller, due to its access to controlling variables

	// "help" and "reload" are to be handled by MCS, so that they work no matter what
	if (split[0] == "help")
	{
		PrintHelp(split, a_Output);
		a_Output.Finished();
		return;
	}
	else if (split[0] == "reload")
	{
		cPluginManager::Get()->ReloadPlugins();
		a_Output.Finished();
		return;
	}
	else if (split[0] == "reloadplugins")
	{
		cPluginManager::Get()->ReloadPlugins();
		a_Output.Out("Plugins reloaded");
		a_Output.Finished();
		return;
	}
	else if (split[0] == "load")
	{
		if (split.size() > 1)
		{
			cPluginManager::Get()->RefreshPluginList();  // Refresh the plugin list, so that if the plugin was added just now, it is loadable
			a_Output.Out(cPluginManager::Get()->LoadPlugin(split[1]) ? "Plugin loaded" : "Error occurred loading plugin");
		}
		else
		{
			a_Output.Out("Usage: load <PluginFolder>");
		}
		a_Output.Finished();
		return;
	}
	else if (split[0] == "unload")
	{
		if (split.size() > 1)
		{
			cPluginManager::Get()->UnloadPlugin(split[1]);
			a_Output.Out("Plugin unload scheduled");
		}
		else
		{
			a_Output.Out("Usage: unload <PluginFolder>");
		}
		a_Output.Finished();
		return;
	}
	if (split[0] == "destroyentities")
	{
		class WorldCallback : public cWorldListCallback
		{
			virtual bool Item(cWorld * a_World) override
			{
				class EntityCallback : public cEntityCallback
				{
					virtual bool Item(cEntity * a_Entity) override
					{
						if (!a_Entity->IsPlayer())
						{
							a_Entity->Destroy();
						}
						return false;
					}
				} EC;
				a_World->ForEachEntity(EC);
				return false;
			}
		} WC;
		cRoot::Get()->ForEachWorld(WC);
		a_Output.Out("Destroyed all entities");
		a_Output.Finished();
		return;
	}

	// There is currently no way a plugin can do these (and probably won't ever be):
	else if (split[0].compare("chunkstats") == 0)
	{
		cRoot::Get()->LogChunkStats(a_Output);
		a_Output.Finished();
		return;
	}

	else if (split[0].compare("luastats") == 0)
	{
		a_Output.Out(cLuaStateTracker::GetStats());
		a_Output.Finished();
		return;
	}
	else if (cPluginManager::Get()->ExecuteConsoleCommand(split, a_Output, a_Cmd))
	{
		a_Output.Finished();
		return;
	}

	a_Output.Out("Unknown command, type 'help' for all commands.");
	a_Output.Finished();
}





void cServer::PrintHelp(const AStringVector & a_Split, cCommandOutputCallback & a_Output)
{
	UNUSED(a_Split);
	typedef std::pair<AString, AString> AStringPair;
	typedef std::vector<AStringPair> AStringPairs;

	class cCallback :
		public cPluginManager::cCommandEnumCallback
	{
	public:
		cCallback(void) : m_MaxLen(0) {}

		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
		UNUSED(a_Plugin);
		UNUSED(a_Permission);
			if (!a_HelpString.empty())
			{
				m_Commands.push_back(AStringPair(a_Command, a_HelpString));
				if (m_MaxLen < a_Command.length())
				{
					m_MaxLen = a_Command.length();
				}
			}
			return false;
		}

		AStringPairs m_Commands;
		size_t m_MaxLen;
	} Callback;
	cPluginManager::Get()->ForEachConsoleCommand(Callback);
	std::sort(Callback.m_Commands.begin(), Callback.m_Commands.end());
	for (AStringPairs::const_iterator itr = Callback.m_Commands.begin(), end = Callback.m_Commands.end(); itr != end; ++itr)
	{
		const AStringPair & cmd = *itr;
		a_Output.Out(Printf("%-*s - %s\n", static_cast<int>(Callback.m_MaxLen), cmd.first.c_str(), cmd.second.c_str()));
	}  // for itr - Callback.m_Commands[]
}





void cServer::BindBuiltInConsoleCommands(void)
{
	// Create an empty handler - the actual handling for the commands is performed before they are handed off to cPluginManager
	class cEmptyHandler:
		public cPluginManager::cCommandHandler
	{
		virtual bool ExecuteCommand(
			const AStringVector & a_Split,
			cPlayer * a_Player,
			const AString & a_Command,
			cCommandOutputCallback * a_Output = nullptr
		) override
		{
			return false;
		}
	};
	auto handler = std::make_shared<cEmptyHandler>();

	// Register internal commands:
	cPluginManager * PlgMgr = cPluginManager::Get();
	PlgMgr->BindConsoleCommand("help",            nullptr, handler, "Shows the available commands");
	PlgMgr->BindConsoleCommand("reload",          nullptr, handler, "Reloads all plugins");
	PlgMgr->BindConsoleCommand("restart",         nullptr, handler, "Restarts the server cleanly");
	PlgMgr->BindConsoleCommand("stop",            nullptr, handler, "Stops the server cleanly");
	PlgMgr->BindConsoleCommand("chunkstats",      nullptr, handler, "Displays detailed chunk memory statistics");
	PlgMgr->BindConsoleCommand("load",            nullptr, handler, "Adds and enables the specified plugin");
	PlgMgr->BindConsoleCommand("unload",          nullptr, handler, "Disables the specified plugin");
	PlgMgr->BindConsoleCommand("destroyentities", nullptr, handler, "Destroys all entities in all worlds");
}





void cServer::Shutdown(void)
{
	// Stop listening on all sockets:
	for (auto srv: m_ServerHandles)
	{
		srv->Close();
	}
	m_ServerHandles.clear();

	// Notify the tick thread and wait for it to terminate:
	m_bRestarting = true;
	m_RestartEvent.Wait();

	cRoot::Get()->SaveAllChunks();

	// Remove all clients:
	cCSLock Lock(m_CSClients);
	for (auto itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		(*itr)->Destroy();
	}
	m_Clients.clear();
}





void cServer::KickUser(int a_ClientID, const AString & a_Reason)
{
	cCSLock Lock(m_CSClients);
	for (auto itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		if ((*itr)->GetUniqueID() == a_ClientID)
		{
			(*itr)->Kick(a_Reason);
		}
	}  // for itr - m_Clients[]
}





void cServer::AuthenticateUser(int a_ClientID, const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	cCSLock Lock(m_CSClients);

	// Check max players condition within lock (expect server and authenticator thread to both call here)
	if (GetNumPlayers() >= GetMaxPlayers())
	{
		KickUser(a_ClientID, "The server is currently full :(" "\n" "Try again later?");
		return;
	}

	for (auto itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		if ((*itr)->GetUniqueID() == a_ClientID)
		{
			(*itr)->Authenticate(a_Name, a_UUID, a_Properties);
			return;
		}
	}  // for itr - m_Clients[]
}





