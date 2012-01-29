#pragma once

#include "../WebServer/WebServer.h"
#include "cSocket.h"

class cStringMap;
//tolua_begin
struct HTTPRequest
{
	std::string Method;
	std::string Path;
	cStringMap* Params;
	std::string Username;
};
//tolua_end

struct lua_State;
class cEvent;
class cIniFile;
class cWebPlugin;
class cWebAdmin
{
public:
	cWebAdmin( int a_Port = 8080 );
	~cWebAdmin();

	bool Init( int a_Port );

	void AddPlugin( cWebPlugin* a_Plugin );
	void RemovePlugin( cWebPlugin* a_Plugin );

	typedef std::list< cWebPlugin* > PluginList;
	PluginList GetPlugins() { return m_Plugins; }

	static void Request_Handler(webserver::http_request* r);

	void RemovePlugin( lua_State* L );
private:

#ifdef _WIN32
	static DWORD WINAPI ListenThread(LPVOID lpParam);
#else
	static void *ListenThread( void *lpParam );
#endif

	std::string GetTemplate();

	int m_Port;

	bool m_bConnected;
	cSocket m_ListenSocket;

	cIniFile* m_IniFile;
	PluginList m_Plugins;

	cEvent* m_Event;

	webserver* m_WebServer;
};