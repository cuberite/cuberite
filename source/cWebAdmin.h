#pragma once

#include "../WebServer/WebServer.h"
#include "cSocket.h"

class cStringMap;

struct HTTPFormData		//tolua_export
{						//tolua_export
	std::string Name;	//tolua_export
	std::string Value;	//tolua_export
	std::string Type;	//tolua_export
};//tolua_export

struct HTTPRequest	//tolua_export
{					//tolua_export
	typedef std::map< std::string, std::string > StringStringMap;
	typedef std::map< std::string, HTTPFormData > FormDataMap;
	std::string Method;			//tolua_export
	std::string Path;			//tolua_export
	StringStringMap Params;		// >> EXPORTED IN MANUALBINDINGS <<
	StringStringMap PostParams;	// >> EXPORTED IN MANUALBINDINGS <<
	std::string Username;		//tolua_export
	FormDataMap FormData;		// >> EXPORTED IN MANUALBINDINGS <<
}; //tolua_export

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